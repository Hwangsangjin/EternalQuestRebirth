// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameMode.h"
#include "EQGameState.h"
#include "Player/EQPlayerState.h"

AEQGameMode::AEQGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnRef(TEXT("/Game/Blueprints/Player/BP_Player.BP_Player_C"));
	if (DefaultPawnRef.Succeeded())
	{
		DefaultPawnClass = DefaultPawnRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerRef(TEXT("/Game/Blueprints/Player/BP_PlayerController.BP_PlayerController_C"));
	if (PlayerControllerRef.Succeeded())
	{
		PlayerControllerClass = PlayerControllerRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AEQGameState> GameStateRef(TEXT("/Game/Blueprints/Game/BP_GameState.BP_GameState_C"));
	if (GameStateRef.Succeeded())
	{
		GameStateClass = GameStateRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AEQPlayerState> PlayerStateRef(TEXT("/Game/Blueprints/Player/BP_PlayerState.BP_PlayerState_C"));
	if (PlayerStateRef.Succeeded())
	{
		PlayerStateClass = PlayerStateRef.Class;
	}
}
