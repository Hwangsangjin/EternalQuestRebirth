// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Character/EQBaseCharacter.h"
#include "Data/EQComboActionData.h"
#include "Equipment/EQWeapon.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Interface/EQCombatCapable.h"

UEQGameplayAbility_Attack::UEQGameplayAbility_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboIndex = 0;
}

bool UEQGameplayAbility_Attack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	return AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::Character_State_Combat);
}

void UEQGameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		PlayNotEnoughStaminaCue();
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AEQBaseCharacter* Character = Cast<AEQBaseCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ComboActionData = Character->GetComboActionData();
	if (!ComboActionData)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	const IEQCombatCapable* Interface = Cast<IEQCombatCapable>(Character);
	if (!Interface)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AEQWeapon* Weapon = Interface->GetWeapon();
	if (!Weapon)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	MontageToPlay = Weapon->GetMontageForTag(EQGameplayTag::Character_Attack_Light, 0);

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), MontageToPlay, 1.0f, GetNextSectionName());
	PlayAttackTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompletedCallback);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayAttackTask->ReadyForActivation();

	SendAttackPressedTagToUI();
	StartComboTimer();
}

void UEQGameplayAbility_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UEQGameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentComboIndex = 0;
	bHasNextComboInput = false;
	ComboActionData = nullptr;
}

bool UEQGameplayAbility_Attack::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	return HasEnoughStamina();
}

void UEQGameplayAbility_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	SendAttackPressedTagToUI();

	if (!HasEnoughStamina())
	{
		bHasNextComboInput = false;
		PlayNotEnoughStaminaCue();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		bHasNextComboInput = false;
	}
	else
	{
		bHasNextComboInput = true;
	}
}

void UEQGameplayAbility_Attack::OnCompletedCallback()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEQGameplayAbility_Attack::OnInterruptedCallback()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FName UEQGameplayAbility_Attack::GetNextSectionName()
{
	CurrentComboIndex = FMath::Clamp(CurrentComboIndex + 1, 1, ComboActionData->MaxComboCount);
	const FName NextSectionName = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentComboIndex);
	return NextSectionName;
}

void UEQGameplayAbility_Attack::StartComboTimer()
{
	const int32 ComboIndex = CurrentComboIndex - 1;

	const float ComboEffectiveTime = ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate;
	if (ComboEffectiveTime <= 0.0f)
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ThisClass::CheckComboInput, ComboEffectiveTime, false);
}

void UEQGameplayAbility_Attack::CheckComboInput()
{
	ComboTimerHandle.Invalidate();

	if (!bHasNextComboInput)
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		return;
	}

	check(CostGameplayEffectClass);

	if (CurrentComboIndex >= 1)
	{
		const FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle CostSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CostGameplayEffectClass, GetAbilityLevel(), ContextHandle);

		if (CostSpecHandle.IsValid())
		{
			GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*CostSpecHandle.Data.Get());
		}
	}

	MontageJumpToSection(GetNextSectionName());
	StartComboTimer();
	bHasNextComboInput = false;
}

bool UEQGameplayAbility_Attack::HasEnoughStamina() const
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

void UEQGameplayAbility_Attack::PlayNotEnoughStaminaCue() const
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

void UEQGameplayAbility_Attack::SendAttackPressedTagToUI() const
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
