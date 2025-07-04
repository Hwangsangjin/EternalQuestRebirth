// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameplayTag/EQGameplayTag.h"
#include "EQStateComponent.generated.h"

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQStateComponent();

	bool IsCurrentStateEqualToAny(const FGameplayTagContainer& GameplayTagContainer) const;

	void ClearState();

	void ToggleMovementInput(bool bEnabled, float Duration = 0.0f);

	FORCEINLINE bool IsMovementInputEnabled() const { return bMovementInputEnabled; }

	FORCEINLINE FGameplayTag GetCurrentState() const { return CurrentState; }
	FORCEINLINE void SetState(const FGameplayTag& NewState)
	{
		if (CurrentState == EQGameplayTag::Character_State_Dead)
		{
			return;
		}

		CurrentState = NewState;
	}

protected:
	UFUNCTION()
	void MovementInputEnableAction();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	FGameplayTag CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	uint8 bMovementInputEnabled : 1;
};
