// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_Interact.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilityTask/EQAbilityTask_Trace.h"
#include "Interface/EQCharacterInteractable.h"
#include "Physics/EQCollision.h"

UEQGameplayAbility_Interact::UEQGameplayAbility_Interact()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ObjectType = UEngineTypes::ConvertToObjectType(CCHANNEL_EQPICKUP);
	TraceRadius = 100.0f;
}

void UEQGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	UEQAbilityTask_Trace* InteractTraceTask = UEQAbilityTask_Trace::CreateTask(this, TargetActorClass);
	InteractTraceTask->TraceResultDelegate.AddDynamic(this, &ThisClass::OnTraceResultCallback);
	InteractTraceTask->ReadyForActivation();
}

void UEQGameplayAbility_Interact::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (TargetDataHandle.Num() == 0)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const FGameplayAbilityTargetData* Data = TargetDataHandle.Get(0);
	if (!Data)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const FHitResult* HitResult = Data->GetHitResult();
	if (!HitResult || !HitResult->GetActor())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AActor* HitActor = HitResult->GetActor();

	if (IEQCharacterInteractable* InteractInterface = Cast<IEQCharacterInteractable>(HitActor))
	{
		AActor* Interactor = CurrentActorInfo->AvatarActor.Get();
		InteractInterface->PerformInteract(Interactor);
	}

	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
