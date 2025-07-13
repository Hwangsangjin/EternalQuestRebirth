// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EQAnimNotifyState_CharacterRotate.generated.h"

UCLASS()
class ETERNALQUEST_API UEQAnimNotifyState_CharacterRotate : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UEQAnimNotifyState_CharacterRotate();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	UPROPERTY()
	TObjectPtr<AController> Controller;

	FRotator InitialAttackRotation;
};
