// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EQCharacter.generated.h"

UCLASS()
class ETERNALQUEST_API AEQCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEQCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
