// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotify_DrinkPotion.h"
#include "Component/EQInventoryComponent.h"

UEQAnimNotify_DrinkPotion::UEQAnimNotify_DrinkPotion(const FObjectInitializer& ObjectInitializer)
{
}

void UEQAnimNotify_DrinkPotion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	const AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	UEQInventoryComponent* InventoryComponent = Owner->GetComponentByClass<UEQInventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}

	InventoryComponent->DrinkPotion();
}
