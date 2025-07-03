// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQEquipment.h"
#include "GameplayTagContainer.h"
#include "EQWeapon.generated.h"

enum class ECombatType : uint8;
class UEQCollisionComponent;
class UEQMontageActionData;
class UEQCombatComponent;

UCLASS()
class ETERNALQUEST_API AEQWeapon : public AEQEquipment
{
	GENERATED_BODY()

public:
	AEQWeapon();

	virtual void EquipItem() override;
	virtual void UnequipItem() override;

	virtual void ActivateCollision();
	virtual void DeactivateCollision();

	UAnimMontage* GetMontageForTag(const FGameplayTag& GameplayTag, const int32 Index = 0) const;
	UAnimMontage* GetRandomMontageForTag(const FGameplayTag& GameplayTag) const;
	UAnimMontage* GetHitReactionMontage(const AActor* Attacker) const;

	FORCEINLINE FName GetEquipSocketName() const { return EquipSocketName; }
	FORCEINLINE FName GetUnequipSocketName() const { return UnequipSocketName; }

	FORCEINLINE UEQCollisionComponent* GetCollisionComponent() const { return CollisionComponent; }

	FORCEINLINE ECombatType GetCombatType() const { return CombatType; }

	void OnHitCallback(const FHitResult& HitResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Socket, Meta = (AllowPrivateAccess = true))
	FName EquipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Socket, Meta = (AllowPrivateAccess = true))
	FName UnequipSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQMontageActionData> MontageActionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQCollisionComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = true))
	ECombatType CombatType;
};
