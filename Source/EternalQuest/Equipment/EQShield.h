// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQEquipment.h"
#include "EQShield.generated.h"

class UEQCombatComponent;

UCLASS()
class ETERNALQUEST_API AEQShield : public AEQEquipment
{
	GENERATED_BODY()

public:
	AEQShield();

	virtual void EquipItem() override;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Socket, Meta=(AllowPrivateAccess = true))
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Socket, Meta=(AllowPrivateAccess = true))
	FName UnequipSocketName;
};
