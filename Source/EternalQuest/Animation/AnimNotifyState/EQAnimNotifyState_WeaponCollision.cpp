// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotifyState_WeaponCollision.h"
#include "Interface/EQCombatCapable.h"

UEQAnimNotifyState_WeaponCollision::UEQAnimNotifyState_WeaponCollision(const FObjectInitializer& ObjectInitializer)
{
}

void UEQAnimNotifyState_WeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

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

	Interface->ActivateWeaponCollision();
}

void UEQAnimNotifyState_WeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

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

	Interface->ActivateWeaponCollision();
}
