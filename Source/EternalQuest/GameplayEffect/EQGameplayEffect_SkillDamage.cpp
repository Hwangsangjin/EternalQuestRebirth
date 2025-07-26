// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayEffect/EQGameplayEffect_SkillDamage.h"
#include "AbilitySystemComponent.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Attribute/EQCharacterSkillAttributeSet.h"

UEQGameplayEffect_SkillDamage::UEQGameplayEffect_SkillDamage()
{
}

void UEQGameplayEffect_SkillDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	const UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	if (!SourceAbilitySystemComponent || !TargetAbilitySystemComponent)
	{
		return;
	}

	const AActor* SourceActor = SourceAbilitySystemComponent->GetAvatarActor();
	const AActor* TargetActor = TargetAbilitySystemComponent->GetAvatarActor();

	if (!SourceActor || !TargetActor)
	{
		return;
	}

	const float MaxDamageRange = SourceAbilitySystemComponent->GetNumericAttributeBase(UEQCharacterSkillAttributeSet::GetMaxSkillRangeAttribute());
	const float MaxDamage = SourceAbilitySystemComponent->GetNumericAttributeBase(UEQCharacterSkillAttributeSet::GetSkillRateAttribute());
	const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);
	const float InvDamageRatio = 1.0f - Distance / MaxDamageRange;
	const float Damage = InvDamageRatio * MaxDamage;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UEQCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
}
