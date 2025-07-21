// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_ToggleCombat.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/EQBaseCharacter.h"
#include "Equipment/EQWeapon.h"
#include "GameplayTag/EQGameplayTag.h"

UEQGameplayAbility_ToggleCombat::UEQGameplayAbility_ToggleCombat()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UEQGameplayAbility_ToggleCombat::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::Character_State_ToggleCombat))
	{
		return false;
	}

	AEQBaseCharacter* Character = Cast<AEQBaseCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return false;
	}

	const IEQCombatCapable* Interface = Cast<IEQCombatCapable>(Character);
	if (!Interface)
	{
		return false;
	}

	const AEQWeapon* Weapon = Interface->GetWeapon();
	if (!Weapon)
	{
		return false;
	}

	return true;
}

void UEQGameplayAbility_ToggleCombat::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::Character_State_ToggleCooldown) || AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::Character_State_ToggleCombat))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AEQBaseCharacter* Character = Cast<AEQBaseCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	IEQCombatCapable* Interface = Cast<IEQCombatCapable>(Character);
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

	AbilitySystemComponent->AddLooseGameplayTag(EQGameplayTag::Character_State_ToggleCombat);

	bShouldEnterCombat = !Interface->IsCombatEnabled();
	Interface->SetCombatEnabled(bShouldEnterCombat);

	if (bShouldEnterCombat)
	{
		MontageToPlay = Weapon->GetMontageForTag(EQGameplayTag::Character_Action_Equip, 0);
	}
	else
	{
		MontageToPlay = Weapon->GetMontageForTag(EQGameplayTag::Character_Action_Unequip, 0);
	}

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayToggleCombatTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ToggleCombatMontage"), MontageToPlay);
	PlayToggleCombatTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompletedCallback);
	PlayToggleCombatTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayToggleCombatTask->ReadyForActivation();
}

void UEQGameplayAbility_ToggleCombat::OnCompletedCallback()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::Character_State_ToggleCombat);

	if (bShouldEnterCombat)
	{
		AbilitySystemComponent->AddLooseGameplayTag(EQGameplayTag::Character_State_Combat);
	}
	else
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::Character_State_Combat);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UEQGameplayAbility_ToggleCombat::OnInterruptedCallback()
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystemComponent)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::Character_State_ToggleCombat);

	AEQBaseCharacter* Character = Cast<AEQBaseCharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	IEQCombatCapable* Interface = Cast<IEQCombatCapable>(Character);
	if (!Interface)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	Interface->SetCombatEnabled(!bShouldEnterCombat);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
