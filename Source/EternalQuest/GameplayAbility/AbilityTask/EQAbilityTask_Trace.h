// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "EQAbilityTask_Trace.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraceResultDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

class AEQTargetActor_Trace;

UCLASS()
class ETERNALQUEST_API UEQAbilityTask_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UEQAbilityTask_Trace();

	UFUNCTION()
	static UEQAbilityTask_Trace* CreateTask(UGameplayAbility* OwningAbility, const TSubclassOf<AEQTargetActor_Trace>& TargetActorClass);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY()
	FTraceResultDelegate TraceResultDelegate;

private:
	UPROPERTY()
	TSubclassOf<AEQTargetActor_Trace> TargetActorClass;

	UPROPERTY()
	TObjectPtr<AEQTargetActor_Trace> SpawnedTargetActor;
};
