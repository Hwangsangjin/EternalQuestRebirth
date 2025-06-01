// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPlayerController.h"
#include "UI/EQPlayerHUDWidget.h"

AEQPlayerController::AEQPlayerController()
{
	static ConstructorHelpers::FClassFinder<UEQPlayerHUDWidget> PlayerHUDWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	if (PlayerHUDWidgetClassRef.Succeeded())
	{
		PlayerHUDWidgetClass = PlayerHUDWidgetClassRef.Class;
	}
}

void AEQPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);

	if (!PlayerHUDWidgetClass)
	{
		return;
	}

	PlayerHUDWidget = CreateWidget<UEQPlayerHUDWidget>(this, PlayerHUDWidgetClass);
	if (!PlayerHUDWidget)
	{
		return;
	}

	PlayerHUDWidget->AddToViewport();
}
