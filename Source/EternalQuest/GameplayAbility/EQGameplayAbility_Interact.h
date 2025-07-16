// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "EQGameplayAbility_Interact.generated.h"

class AEQTargetActor_Trace;

UCLASS()
class ETERNALQUEST_API UEQGameplayAbility_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UEQGameplayAbility_Interact();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UFUNCTION()
	void OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQTargetActor_Trace> TargetActorClass;

	UPROPERTY(EditAnywhere, Category = Trace, Meta = (AllowPrivateAccess = true))
	TEnumAsByte<EObjectTypeQuery> ObjectType;

	UPROPERTY(EditAnywhere, Category = Trace, Meta = (AllowPrivateAccess = true))
	float TraceRadius;
};
