// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQAttributeComponent.generated.h"

UENUM(BlueprintType)
enum class EEQAttributeType : uint8
{
	Health	UMETA(DisplayName = "Health"),
	Stamina	UMETA(DisplayName = "Stamina")
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedDelegate, EEQAttributeType, float);
DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQAttributeComponent();

	FORCEINLINE float GetBaseHealth() const { return BaseHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	FORCEINLINE float GetHealthRatio() const { return BaseHealth / MaxHealth; }
	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; }

	bool CheckHasEnoughStamina(float StaminaCost) const;
	void DecreaseStamina(float StaminaCost);
	void ToggleRegainStamina(bool bEnabled);
	void RegainStamina();

	void BroadcastAttributeChanged(EEQAttributeType InAttributeType) const;
	FOnAttributeChangedDelegate OnAttributeChangedDelegate;

	void TakeDamageAmount(float DamageAmount);

	void Heal(float HealAmount);

	FOnDeadDelegate OnDeadDelegate;

protected:
	virtual void BeginPlay() override;

	void ModifyStamina(float Delta);

private:
	UPROPERTY(VisibleInstanceOnly, Category = Stamina, Meta = (AllowPrivateAccess = true))
	float BaseStamina;

	UPROPERTY(VisibleInstanceOnly, Category = Stamina, Meta = (AllowPrivateAccess = true))
	float MaxStamina;

	UPROPERTY(VisibleInstanceOnly, Category = Stamina, Meta = (AllowPrivateAccess = true))
	float RegainRate;

	UPROPERTY(VisibleInstanceOnly, Category = Health, Meta = (AllowPrivateAccess = true))
	float BaseHealth;

	UPROPERTY(VisibleInstanceOnly, Category = Health, Meta = (AllowPrivateAccess = true))
	float MaxHealth;

	FTimerHandle RegainStaminaTimerHandle;
};
