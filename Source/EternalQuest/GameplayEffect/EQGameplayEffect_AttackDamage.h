// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EQGameplayEffect_AttackDamage.generated.h"

UCLASS()
class ETERNALQUEST_API UEQGameplayEffect_AttackDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UEQGameplayEffect_AttackDamage();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
