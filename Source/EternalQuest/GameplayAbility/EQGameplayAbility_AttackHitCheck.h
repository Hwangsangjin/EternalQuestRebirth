// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EQGameplayAbility_AttackHitCheck.generated.h"

class UEQCharacterAttributeSet;
class AEQTargetActor_Trace;

UCLASS()
class ETERNALQUEST_API UEQGameplayAbility_AttackHitCheck : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEQGameplayAbility_AttackHitCheck();

	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQTargetActor_Trace> TargetActorClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> AttackBuffEffect;

	UPROPERTY(EditAnywhere, Category = Data, Meta = (AllowPrivateAccess = true))
	float CurrentComboLevel;
};
