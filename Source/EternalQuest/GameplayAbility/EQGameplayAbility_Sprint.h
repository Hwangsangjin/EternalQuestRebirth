// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EQGameplayAbility_Sprint.generated.h"

UCLASS()
class ETERNALQUEST_API UEQGameplayAbility_Sprint : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEQGameplayAbility_Sprint();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UPROPERTY(EditAnywhere, Category = Sprint, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> SprintStaminaDrainEffect;

	UPROPERTY(EditAnywhere, Category = Sprint, Meta = (AllowPrivateAccess = true))
	float SprintSpeed;

	UPROPERTY(EditAnywhere, Category = Sprint, Meta = (AllowPrivateAccess = true))
	float NormalSpeed;

	FActiveGameplayEffectHandle StaminaDrainHandle;
	FGameplayAbilitySpecHandle StaminaRegenAbilityHandle;
};
