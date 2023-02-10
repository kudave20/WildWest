// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "WildWest/GameState/LobbyGameState.h"

void ALobbyPlayerController::ServerGunmanButtonClicked_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->ServerGunmanButtonClicked();
		}
	}
}

void ALobbyPlayerController::ServerSheriffButtonClicked_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->ServerSheriffButtonClicked();
		}
	}
}
