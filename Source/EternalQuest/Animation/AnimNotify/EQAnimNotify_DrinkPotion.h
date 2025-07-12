// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EQAnimNotify_DrinkPotion.generated.h"

UCLASS()
class ETERNALQUEST_API UEQAnimNotify_DrinkPotion : public UAnimNotify
{
	GENERATED_BODY()

public:
	explicit UEQAnimNotify_DrinkPotion(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
