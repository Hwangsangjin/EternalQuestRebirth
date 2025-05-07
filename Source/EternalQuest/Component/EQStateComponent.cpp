// Fill out your copyright notice in the Description page of Project Settings.

#include "EQStateComponent.h"
#include "EternalQuestGameplayTags.h"

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
	if (CurrentState == EternalQuestGameplayTags::Character_State_Death)
	{
		return;
	}

	CurrentState = FGameplayTag::EmptyTag;
}
