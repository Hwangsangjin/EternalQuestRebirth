// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayCueNotify_InteractPrompt.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Player/EQPlayerController.h"
#include "UI/EQHUDWidget.h"

UEQGameplayCueNotify_InteractPrompt::UEQGameplayCueNotify_InteractPrompt()
{
}

bool UEQGameplayCueNotify_InteractPrompt::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const APawn* Pawn = Cast<APawn>(MyTarget);
	const APlayerController* PlayerController = Pawn ? Cast<APlayerController>(Pawn->GetController()) : nullptr;
	if (!PlayerController)
	{
		return false;
	}

	const AEQPlayerController* EQPlayerController = Cast<AEQPlayerController>(PlayerController);
	if (!EQPlayerController)
	{
		return false;
	}

	const UEQHUDWidget* HUDWidget = EQPlayerController->GetHUDWidget();
	if (!HUDWidget)
	{
		return false;
	}

	UEQUserWidget* InteractPromptWidget = HUDWidget->GetInteractPromptWidget();
	if (!InteractPromptWidget)
	{
		return false;
	}

	if (GameplayCueTag == EQGameplayTag::GameplayCue_InteractPrompt_Show)
	{
		InteractPromptWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else if (GameplayCueTag == EQGameplayTag::GameplayCue_InteractPrompt_Hide)
	{
		InteractPromptWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	return true;
}
