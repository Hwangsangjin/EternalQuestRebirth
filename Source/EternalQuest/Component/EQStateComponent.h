// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "EQStateComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQStateComponent();

	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& GameplayTagContainer) const;

	FORCEINLINE FGameplayTag GetCurrentState() { return CurrentState; }
	FORCEINLINE void SetState(const FGameplayTag& NewState) { CurrentState = NewState; }
	void ClearState();

	FORCEINLINE bool IsMovementInputEnabled() const { return bMovementInputEnabled; }
	FORCEINLINE void ToggleMovementInput(bool bEnabled) { bMovementInputEnabled = bEnabled; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = true))
	FGameplayTag CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = true))
	uint8 bMovementInputEnabled : 1;
};
