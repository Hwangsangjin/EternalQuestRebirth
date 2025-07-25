// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayCueNotify_WarningPrompt.h"
#include "Player/EQPlayerController.h"
#include "UI/EQHUDWidget.h"

UEQGameplayCueNotify_WarningPrompt::UEQGameplayCueNotify_WarningPrompt()
{
}

bool UEQGameplayCueNotify_WarningPrompt::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
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

	UEQUserWidget* WarningPromptWidget = HUDWidget->GetWarningPromptWidget();
	if (!WarningPromptWidget)
	{
		return false;
	}

	WarningPromptWidget->SetVisibility(ESlateVisibility::Visible);

	TWeakObjectPtr WeakWarningPromptWidget = WarningPromptWidget;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakWarningPromptWidget]()
	{
		if (WeakWarningPromptWidget.IsValid())
		{
			WeakWarningPromptWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}, 1.5f, false);

	return true;
}
