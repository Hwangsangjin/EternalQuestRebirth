// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayEffect_AttackDamage.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Character/EQBaseCharacter.h"
#include "Component/EQStateComponent.h"
#include "GameplayTag/EQGameplayTag.h"

UEQGameplayEffect_AttackDamage::UEQGameplayEffect_AttackDamage()
{
}

void UEQGameplayEffect_AttackDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	if (!TargetActor)
	{
		return;
	}

	AEQBaseCharacter* TargetCharacter = Cast<AEQBaseCharacter>(TargetActor);
	if (!TargetCharacter)
	{
		return;
	}
}
