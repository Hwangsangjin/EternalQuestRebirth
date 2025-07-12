// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EQAnimNotify_AttackHitCheck.generated.h"

UCLASS()
class ETERNALQUEST_API UEQAnimNotify_AttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UEQAnimNotify_AttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = GameplayTag, Meta = (AllowPrivateAccess = true))
	FGameplayTag TriggerGameplayTag;

	UPROPERTY(EditAnywhere, Category = Data, Meta = (AllowPrivateAccess = true))
	float ComboLevel;
};
