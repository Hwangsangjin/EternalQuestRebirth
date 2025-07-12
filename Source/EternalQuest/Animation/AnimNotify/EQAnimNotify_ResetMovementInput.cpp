// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotify_ResetMovementInput.h"
#include "Component/EQStateComponent.h"

void UEQAnimNotify_ResetMovementInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	const AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	UEQStateComponent* StateComponent = Owner->GetComponentByClass<UEQStateComponent>();
	if (!StateComponent)
	{
		return;
	}

	StateComponent->ToggleMovementInput(true);
}
