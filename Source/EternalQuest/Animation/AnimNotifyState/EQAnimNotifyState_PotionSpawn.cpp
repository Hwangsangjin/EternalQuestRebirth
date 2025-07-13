// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotifyState_PotionSpawn.h"
#include "Component/EQCombatComponent.h"
#include "Component/EQInventoryComponent.h"
#include "Equipment/EQShield.h"

UEQAnimNotifyState_PotionSpawn::UEQAnimNotifyState_PotionSpawn(const FObjectInitializer& ObjectInitializer)
{
}

void UEQAnimNotifyState_PotionSpawn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

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

	InventoryComponent->SpawnPotion();

	const UEQCombatComponent* CombatComponent = Owner->GetComponentByClass<UEQCombatComponent>();
	if (!CombatComponent)
	{
		return;
	}

	AEQShield* Shield = CombatComponent->GetShield();
	if (!Shield)
	{
		return;
	}

	Shield->SetActorHiddenInGame(true);
}

void UEQAnimNotifyState_PotionSpawn::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

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

	InventoryComponent->DespawnPotion();

	const UEQCombatComponent* CombatComponent = Owner->GetComponentByClass<UEQCombatComponent>();
	if (!CombatComponent)
	{
		return;
	}

	AEQShield* Shield = CombatComponent->GetShield();
	if (!Shield)
	{
		return;
	}

	Shield->SetActorHiddenInGame(false);
}
