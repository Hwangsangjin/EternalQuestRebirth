// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotify_EquipWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Equipment/EQWeapon.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Interface/EQCombatCapable.h"

class IEQCombatCapable;

UEQAnimNotify_EquipWeapon::UEQAnimNotify_EquipWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEQAnimNotify_EquipWeapon::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	IEQCombatCapable* Interface = Cast<IEQCombatCapable>(Owner);
	if (!Interface)
	{
		return;
	}

	AEQWeapon* Weapon = Interface->GetWeapon();
	if (!Weapon)
	{
		return;
	}

	bool bCombatEnabled = Interface->IsCombatEnabled();

	FName SocketName;

	if (MontageActionTag == EQGameplayTag::Character_Action_Equip)
	{
		bCombatEnabled = true;
		SocketName = Weapon->GetEquipSocketName();
	}
	else if (MontageActionTag == EQGameplayTag::Character_Action_Unequip)
	{
		bCombatEnabled = false;
		SocketName = Weapon->GetUnequipSocketName();
	}

	Interface->SetCombatEnabled(bCombatEnabled);
	Weapon->AttachToOwner(SocketName);
}
