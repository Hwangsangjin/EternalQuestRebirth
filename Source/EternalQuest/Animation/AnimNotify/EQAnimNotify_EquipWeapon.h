// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EQAnimNotify_EquipWeapon.generated.h"

UCLASS()
class ETERNALQUEST_API UEQAnimNotify_EquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	explicit UEQAnimNotify_EquipWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tag, Meta = (AllowPrivateAccess = true))
	FGameplayTag MontageActionTag;
};
