// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EternalQuestDefine.h"
#include "Components/ActorComponent.h"
#include "EQAttributeComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, EEQAttributeType, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQAttributeComponent();

	FORCEINLINE float GetBaseStamina() const { return BaseStamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	FORCEINLINE float GetStaminaRatio() const { return BaseStamina / MaxStamina; }

	bool CheckHasEnoughStamina(float StaminaCost) const;
	void DecreaseStamina(float StaminaCost);
	void ToggleRegainStamina(bool bEnabled);
	void RegainStamina();

	void BroadcastAttributeChanged(EEQAttributeType InAttributeType) const;
	FOnAttributeChanged OnAttributeChanged;

protected:
	virtual void BeginPlay() override;

	void ModifyStamina(float Delta);

private:
	UPROPERTY(VisibleInstanceOnly, Category = Stamina, meta = (AllowPrivateAccess = true))
	float BaseStamina;

	UPROPERTY(VisibleInstanceOnly, Category = Stamina, meta = (AllowPrivateAccess = true))
	float MaxStamina;

	UPROPERTY(VisibleInstanceOnly, Category = Stamina, meta = (AllowPrivateAccess = true))
	float RegainRate;

	FTimerHandle RegainStaminaTimerHandle;
};
