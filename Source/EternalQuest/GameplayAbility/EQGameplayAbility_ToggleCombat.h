// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EQGameplayAbility_ToggleCombat.generated.h"

UCLASS()
class ETERNALQUEST_API UEQGameplayAbility_ToggleCombat : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEQGameplayAbility_ToggleCombat();

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnCompletedCallback();

	UFUNCTION()
	void OnInterruptedCallback();

private:
	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	bool bShouldEnterCombat;
};
