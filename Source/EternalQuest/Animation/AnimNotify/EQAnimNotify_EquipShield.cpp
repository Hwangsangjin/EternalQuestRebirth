// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotify_EquipShield.h"
#include "Equipment/EQShield.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Interface/EQCombatCapable.h"

UEQAnimNotify_EquipShield::UEQAnimNotify_EquipShield(const FObjectInitializer& ObjectInitializer)
{
}

void UEQAnimNotify_EquipShield::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

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

	AEQShield* Shield = Interface->GetShield();
	if (!Shield)
	{
		return;
	}

	bool bCombatEnabled = Interface->IsCombatEnabled();

	FName SocketName;

	if (MontageActionTag == EQGameplayTag::Character_Action_Equip)
	{
		bCombatEnabled = true;
		SocketName = Shield->GetEquipSocketName();
	}
	else if (MontageActionTag == EQGameplayTag::Character_Action_Unequip)
	{
		bCombatEnabled = false;
		SocketName = Shield->GetUnequipSocketName();
	}

	Interface->SetCombatEnabled(bCombatEnabled);
	Shield->AttachToOwner(SocketName);
}
