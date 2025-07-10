// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPotion.h"

AEQPotion::AEQPotion()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	SetRootComponent(StaticMeshComponent);
}
