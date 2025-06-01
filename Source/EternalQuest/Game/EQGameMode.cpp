// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameMode.h"

AEQGameMode::AEQGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnRef(TEXT("/Game/Blueprints/Character/BP_Player.BP_Player_C"));
	if (DefaultPawnRef.Succeeded())
	{
		DefaultPawnClass = DefaultPawnRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Game/Blueprints/Player/BP_PlayerController.BP_PlayerController_C"));
	if (PlayerControllerRef.Succeeded())
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}
}
