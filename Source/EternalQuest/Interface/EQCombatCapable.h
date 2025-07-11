// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EQCombatCapable.generated.h"

struct FGameplayTag;
class AEQShield;
class AEQWeapon;

UINTERFACE()
class UEQCombatCapable : public UInterface
{
	GENERATED_BODY()
};

class ETERNALQUEST_API IEQCombatCapable
{
	GENERATED_BODY()

public:
	virtual AEQWeapon* GetWeapon() const = 0;
	virtual AEQShield* GetShield() const = 0;
	virtual bool IsCombatEnabled() const = 0;
	virtual void SetCombatEnabled(bool bEnabled) = 0;
	virtual void ActivateWeaponCollision() = 0;
	virtual void DeactivateWeaponCollision() = 0;

	virtual void PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& OnMontageEnded) {}

	virtual void Parried() {}
};
