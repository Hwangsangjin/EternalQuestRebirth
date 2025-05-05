// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPlayerController.h"
#include "Widget/EQPlayerHUDWidget.h"

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

	if (PlayerHUDWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player HUD Widget Class is nullptr."));
		return;
	}

	PlayerHUDWidget = CreateWidget<UEQPlayerHUDWidget>(this, PlayerHUDWidgetClass);
	if (PlayerHUDWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player HUD Widget is nullptr."));
		return;
	}

	PlayerHUDWidget->AddToViewport();
}
