// Fill out your copyright notice in the Description page of Project Settings.

#include "EQInventoryComponent.h"
#include "EQAttributeComponent.h"
#include "GameFramework/Character.h"
#include "Item/EQPotion.h"

UEQInventoryComponent::UEQInventoryComponent()
{
	PotionQuantity = 10;
	PotionHealAmount = 15.0f;
}

void UEQInventoryComponent::DrinkPotion()
{
	if (PotionQuantity <= 0)
	{
		return;
	}

	UEQAttributeComponent* AttributeComponent = GetOwner()->GetComponentByClass<UEQAttributeComponent>();
	if (!AttributeComponent)
	{
		return;
	}

	--PotionQuantity;
	AttributeComponent->Heal(PotionHealAmount);
}

void UEQInventoryComponent::SpawnPotion()
{
	Potion = GetWorld()->SpawnActor<AEQPotion>(PotionClass, GetOwner()->GetActorTransform());
	if (!Potion)
	{
		return;
	}

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return;
	}

	Potion->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), PotionSocketName);
}

void UEQInventoryComponent::DespawnPotion()
{
	if (!Potion)
	{
		return;
	}

	Potion->Destroy();
}
