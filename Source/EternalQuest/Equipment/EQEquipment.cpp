// Fill out your copyright notice in the Description page of Project Settings.

#include "EQEquipment.h"
#include "GameFramework/Character.h"

AEQEquipment::AEQEquipment()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SetRootComponent(SkeletalMeshComponent);
}

void AEQEquipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!SkeletalMesh)
	{
		return;
	}

	SkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
}

void AEQEquipment::EquipItem()
{
}

void AEQEquipment::UnequipItem()
{
}

void AEQEquipment::AttachToOwner(FName SocketName)
{
	const ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	USkeletalMeshComponent* CharacterMeshComponent = OwnerCharacter->GetMesh();
	if (!CharacterMeshComponent)
	{
		return;
	}

	AttachToComponent(CharacterMeshComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
}
