// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotifyState_CharacterRotate.h"
#include "Component/EQRotationComponent.h"

UEQAnimNotifyState_CharacterRotate::UEQAnimNotifyState_CharacterRotate()
{
}

void UEQAnimNotifyState_CharacterRotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UEQRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UEQRotationComponent>();
	if (!RotationComponent)
	{
		return;
	}

	RotationComponent->ToggleShouldRotate(true);
}

void UEQAnimNotifyState_CharacterRotate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	UEQRotationComponent* RotationComponent = MeshComp->GetOwner()->GetComponentByClass<UEQRotationComponent>();
	if (!RotationComponent)
	{
		return;
	}

	RotationComponent->ToggleShouldRotate(false);
}
