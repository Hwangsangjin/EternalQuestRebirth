// Fill out your copyright notice in the Description page of Project Settings.

#include "EQStateComponent.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Kismet/KismetSystemLibrary.h"

UEQStateComponent::UEQStateComponent()
{
	bMovementInputEnabled = true;
}

bool UEQStateComponent::IsCurrentStateEqualToAny(const FGameplayTagContainer& GameplayTagContainer) const
{
	return GameplayTagContainer.HasTagExact(CurrentState);
}

void UEQStateComponent::ClearState()
{
	if (CurrentState == EQGameplayTag::Character_State_Dead)
	{
		return;
	}

	CurrentState = FGameplayTag::EmptyTag;
}

void UEQStateComponent::ToggleMovementInput(bool bEnabled, float Duration)
{
	if (bEnabled)
	{
		FLatentActionInfo LatentActionInfo;
		LatentActionInfo.CallbackTarget = this;
		LatentActionInfo.ExecutionFunction = "MovementInputEnableAction";
		LatentActionInfo.Linkage = 0;
		LatentActionInfo.UUID = 0;

		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), Duration, LatentActionInfo);
	}
	else
	{
		bMovementInputEnabled = false;
	}
}

void UEQStateComponent::MovementInputEnableAction()
{
	bMovementInputEnabled = true;

	ClearState();
}
