// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "EQTargetActor_Trace.generated.h"

UCLASS()
class ETERNALQUEST_API AEQTargetActor_Trace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AEQTargetActor_Trace();

	FORCEINLINE void SetDebugEnabled(bool bEnable) { bDebugEnabled = bEnable; }

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const;

	bool bDebugEnabled;
};
