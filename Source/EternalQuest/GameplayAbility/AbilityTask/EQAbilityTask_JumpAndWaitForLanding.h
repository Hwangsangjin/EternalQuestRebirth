// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "EQAbilityTask_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpAndWaitForLandingDelegate);

UCLASS()
class ETERNALQUEST_API UEQAbilityTask_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public:
	UEQAbilityTask_JumpAndWaitForLanding();

	static UEQAbilityTask_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	FJumpAndWaitForLandingDelegate JumpAndWaitForLandingDelegate;

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
