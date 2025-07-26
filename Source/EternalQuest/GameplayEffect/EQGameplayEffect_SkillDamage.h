// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EQGameplayEffect_SkillDamage.generated.h"

UCLASS()
class ETERNALQUEST_API UEQGameplayEffect_SkillDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UEQGameplayEffect_SkillDamage();

protected:
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
