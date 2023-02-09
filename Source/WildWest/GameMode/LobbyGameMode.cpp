// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "WildWest/GameState/LobbyGameState.h"

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GameState);

	if (LobbyGameState)
	{
		if (NewPlayer->IsLocalPlayerController())
		{
			LobbyGameState->SetServerPlayerController(NewPlayer);
		}
		else
		{
			LobbyGameState->SetClientPlayerController(NewPlayer);
		}

		NumberOfPlayers++;
		if (NumberOfPlayers == 2)
		{
			LobbyGameState->WidgetSetup();
		}
	}
}

void ALobbyGameMode::TravelToTown()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Town?listen"));
	}
}
