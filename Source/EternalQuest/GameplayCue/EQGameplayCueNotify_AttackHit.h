// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "EQGameplayCueNotify_AttackHit.generated.h"

struct FGameplayEventData;
class UNiagaraSystem;

UCLASS()
class ETERNALQUEST_API UEQGameplayCueNotify_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UEQGameplayCueNotify_AttackHit();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> CameraShakeClass;
};
