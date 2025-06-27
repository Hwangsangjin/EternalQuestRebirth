// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPlayerController.h"
#include "UI/EQHUDWidget.h"

AEQPlayerController::AEQPlayerController()
{
	static ConstructorHelpers::FClassFinder<UEQHUDWidget> HUDWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetClassRef.Succeeded())
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
}

void AEQPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	check(HUDWidgetClass);

	HUDWidget = CreateWidget<UEQHUDWidget>(this, HUDWidgetClass);
	if (!HUDWidget)
	{
		return;
	}

	HUDWidget->AddToViewport();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}
