// Fill out your copyright notice in the Description page of Project Settings.

#include "EQCombatComponent.h"
#include "Character/EQPlayerCharacter.h"
#include "Equipment/EQShield.h"
#include "Equipment/EQWeapon.h"
#include "Item/EQPickupItem.h"

UEQCombatComponent::UEQCombatComponent()
{
}

void UEQCombatComponent::SetWeapon(AEQWeapon* NewWeapon)
{
	if (Weapon)
	{
		SpawnPickupItem(Weapon->GetClass());
		Weapon->Destroy();
	}

	Weapon = NewWeapon;
}

void UEQCombatComponent::SetShield(AEQShield* NewShield)
{
	if (Shield)
	{
		SpawnPickupItem(Shield->GetClass());
		Shield->Destroy();
	}

	Shield = NewShield;
}

void UEQCombatComponent::SetCombatEnabled(const bool bEnabled)
{
	bCombatEnabled = bEnabled;

	if (OnChangedCombatDelegate.IsBound())
	{
		OnChangedCombatDelegate.Broadcast(bCombatEnabled);
	}
}

void UEQCombatComponent::SetBlockingEnabled(const bool bEnabled)
{
	bBlockingEnabled = bEnabled;

	if (OnChangedCombatDelegate.IsBound())
	{
		OnChangedCombatDelegate.Broadcast(bBlockingEnabled);
	}
}

void UEQCombatComponent::SpawnPickupItem(const TSubclassOf<AEQEquipment>& NewEquipmentClass) const
{
	const AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	const FVector SpawnLocation = Owner->GetActorLocation() - Owner->GetActorForwardVector() * 100.0f;
	const FTransform SpawnTransform = FTransform(SpawnLocation);

	AEQPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<AEQPickupItem>(AEQPickupItem::StaticClass(), SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!PickupItem)
	{
		return;
	}

	PickupItem->SetEquipmentClass(NewEquipmentClass);
	PickupItem->FinishSpawning(SpawnTransform);
	PickupItem->Throw();
}
