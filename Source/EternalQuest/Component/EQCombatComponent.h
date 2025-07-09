// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQCombatComponent.generated.h"

class AEQEquipment;
class AEQShield;
class AEQWeapon;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedCombatDelegate, bool);

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQCombatComponent();

	void SetWeapon(AEQWeapon* NewWeapon);
	void SetShield(AEQShield* NewShield);

	void SetCombatEnabled(const bool bEnabled);
	void SetBlockingEnabled(const bool bEnabled);

	FORCEINLINE bool IsCombatEnabled() const { return bCombatEnabled; }
	FORCEINLINE bool IsBlockingEnabled() const { return bBlockingEnabled; }

	FORCEINLINE AEQWeapon* GetWeapon() const { return Weapon; }
	FORCEINLINE AEQShield* GetShield() const { return Shield; }

	FOnChangedCombatDelegate OnChangedCombatDelegate;

private:
	void SpawnPickupItem(const TSubclassOf<AEQEquipment>& NewEquipmentClass) const;

	UPROPERTY()
	TObjectPtr<AEQWeapon> Weapon;

	UPROPERTY()
	TObjectPtr<AEQShield> Shield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	uint8 bCombatEnabled : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	uint8 bBlockingEnabled : 1;
};
