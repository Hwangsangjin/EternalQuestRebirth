// Fill out your copyright notice in the Description page of Project Settings.

#include "EQCharacter.h"

AEQCharacter::AEQCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEQCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEQCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEQCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
