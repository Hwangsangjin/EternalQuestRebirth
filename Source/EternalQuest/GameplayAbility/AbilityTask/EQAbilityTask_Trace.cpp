// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAbilityTask_Trace.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility/TargetActor/EQTargetActor_Trace.h"

UEQAbilityTask_Trace::UEQAbilityTask_Trace()
{
}

UEQAbilityTask_Trace* UEQAbilityTask_Trace::CreateTask(UGameplayAbility* OwningAbility, const TSubclassOf<AEQTargetActor_Trace>& TargetActorClass)
{
	UEQAbilityTask_Trace* NewTask = NewAbilityTask<UEQAbilityTask_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void UEQAbilityTask_Trace::Activate()
{
	Super::Activate();

	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UEQAbilityTask_Trace::OnDestroy(bool bInOwnerFinished)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UEQAbilityTask_Trace::SpawnAndInitializeTargetActor()
{
	if (!Ability || !TargetActorClass)
	{
		EndTask();
		return;
	}

	SpawnedTargetActor = Cast<AEQTargetActor_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (!SpawnedTargetActor)
	{
		EndTask();
		return;
	}

	SpawnedTargetActor->SetDebugEnabled(true);
	SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &ThisClass::OnTargetDataReadyCallback);
}

void UEQAbilityTask_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent* AbilitySystemComponentPtr = AbilitySystemComponent.Get();
	if (!AbilitySystemComponentPtr)
	{
		EndTask();
		return;
	}

	const FTransform SpawnTransform = AbilitySystemComponentPtr->GetAvatarActor()->GetTransform();
	SpawnedTargetActor->FinishSpawning(SpawnTransform);

	AbilitySystemComponentPtr->SpawnedTargetActors.Push(SpawnedTargetActor);
	SpawnedTargetActor->StartTargeting(Ability);
	SpawnedTargetActor->ConfirmTargeting();
}

void UEQAbilityTask_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TraceResultDelegate.Broadcast(TargetDataHandle);
	}

	EndTask();
}
