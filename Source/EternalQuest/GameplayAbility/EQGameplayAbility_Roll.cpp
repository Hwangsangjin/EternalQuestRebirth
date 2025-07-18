// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_Roll.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/EQBaseCharacter.h"

UEQGameplayAbility_Roll::UEQGameplayAbility_Roll()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEQGameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	const AEQBaseCharacter* Character = Cast<AEQBaseCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return;
	}

	RollMontage = Character->GetRollMontage();
	if (!RollMontage)
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("RollMontage"), RollMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompletedCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterruptedCallback);
	PlayMontageTask->ReadyForActivation();
}

void UEQGameplayAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const AEQBaseCharacter* Character = Cast<AEQBaseCharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEQGameplayAbility_Roll::OnCompletedCallback()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEQGameplayAbility_Roll::OnInterruptedCallback()
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
