// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQInventoryComponent.generated.h"

class AEQPotion;

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQInventoryComponent();

	void DrinkPotion();
	void SpawnPotion();
	void DespawnPotion();

	FORCEINLINE int GetPotionQuantity() const { return PotionQuantity; }
	FORCEINLINE void SetPointQuantity(int32 InQuantity) { PotionQuantity = InQuantity; };

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, Meta = (AllowPrivateAccess = true))
	int32 PotionQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, Meta = (AllowPrivateAccess = true))
	float PotionHealAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, Meta = (AllowPrivateAccess = true))
	FName PotionSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQPotion> PotionClass;

	UPROPERTY()
	TObjectPtr<AEQPotion> Potion;
};
