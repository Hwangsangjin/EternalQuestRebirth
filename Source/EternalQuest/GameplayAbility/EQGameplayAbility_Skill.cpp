// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_Skill.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Character/EQBaseCharacter.h"
#include "GameplayTag/EQGameplayTag.h"

UEQGameplayAbility_Skill::UEQGameplayAbility_Skill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UEQGameplayAbility_Skill::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	return AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::Character_State_Combat);
}

void UEQGameplayAbility_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AEQBaseCharacter* Character = CastChecked<AEQBaseCharacter>(ActorInfo->AvatarActor.Get());
	ActiveSkillMontage = Character->GetSkillMontage();
	if (!ActiveSkillMontage)
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompletedCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayMontageTask->ReadyForActivation();

	//SendAttackPressedTagToUI();
}

void UEQGameplayAbility_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEQGameplayAbility_Skill::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	return HasEnoughStamina();
}

void UEQGameplayAbility_Skill::OnCompletedCallback()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEQGameplayAbility_Skill::OnInterruptedCallback()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEQGameplayAbility_Skill::HasEnoughStamina() const
{
	check(CostGameplayEffectClass);

	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	const UGameplayEffect* CostEffectCDO = CostGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	if (!CostEffectCDO)
	{
		return false;
	}

	float RequiredStamina = 0.0f;

	for (const FGameplayModifierInfo& Modifier : CostEffectCDO->Modifiers)
	{
		if (Modifier.Attribute == UEQCharacterAttributeSet::GetStaminaAttribute())
		{
			if (Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(GetAbilityLevel(), RequiredStamina))
			{
				break;
			}
		}
	}

	const float NeededStamina = FMath::Abs(RequiredStamina);
	const float CurrentStamina = AbilitySystemComponent->GetNumericAttribute(UEQCharacterAttributeSet::GetStaminaAttribute());
	return CurrentStamina >= NeededStamina;
}

void UEQGameplayAbility_Skill::PlayNotEnoughStaminaCue() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Avatar);
	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	FGameplayCueParameters Params;
	Params.Instigator = Avatar;
	TargetAbilitySystemComponent->ExecuteGameplayCue(EQGameplayTag::GameplayCue_Character_NotEnoughStamina, Params);
}

void UEQGameplayAbility_Skill::SendAttackPressedTagToUI() const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::UI_Notify_ButtonPressed))
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::UI_Notify_ButtonPressed);
	}

	FTimerHandle TagAddTimer;
	const FTimerDelegate TagAddDelegate = FTimerDelegate::CreateWeakLambda(AbilitySystemComponent, [AbilitySystemComponent]()
	{
		AbilitySystemComponent->AddLooseGameplayTag(EQGameplayTag::UI_Notify_ButtonPressed);
	});

	GetWorld()->GetTimerManager().SetTimer(TagAddTimer, TagAddDelegate, 0.01f, false);

	FTimerHandle TagRemoveTimer;
	const FTimerDelegate TagRemoveDelegate = FTimerDelegate::CreateWeakLambda(AbilitySystemComponent, [AbilitySystemComponent]()
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::UI_Notify_ButtonPressed);
	});

	GetWorld()->GetTimerManager().SetTimer(TagRemoveTimer, TagRemoveDelegate, 0.3f, false);
}
