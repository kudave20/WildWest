// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "WildWest/GameState/LobbyGameState.h"

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		ALobbyGameState* LobbyGameState = Cast<ALobbyGameState>(GameState);
		if (LobbyGameState)
		{
			LobbyGameState->SetbIsLobbyFull(true);
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
