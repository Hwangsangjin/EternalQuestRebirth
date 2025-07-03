// Fill out your copyright notice in the Description page of Project Settings.

#include "EQShield.h"
#include "EQWeapon.h"
#include "Animation/EQAnimInstance.h"
#include "Component/EQCombatComponent.h"

class UEQAnimInstance;

AEQShield::AEQShield()
{
}

void AEQShield::EquipItem()
{
	Super::EquipItem();

	const AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	UEQCombatComponent* CombatComponent = OwnerActor->GetComponentByClass<UEQCombatComponent>();
	if (!CombatComponent)
	{
		return;
	}

	const AEQWeapon* Weapon = CombatComponent->GetWeapon();
	if (!Weapon)
	{
		return;
	}

	CombatComponent->SetShield(this);

	const FName SocketName = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
	AttachToOwner(SocketName);
}
