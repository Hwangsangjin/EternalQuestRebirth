// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EQAnimNotifyState_WeaponCollision.generated.h"

UCLASS()
class ETERNALQUEST_API UEQAnimNotifyState_WeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	explicit UEQAnimNotifyState_WeaponCollision(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
