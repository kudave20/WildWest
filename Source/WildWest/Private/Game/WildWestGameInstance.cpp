// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/WildWestGameInstance.h"
#include "Game/LobbyGameMode.h"
#include "Game/LobbyGameState.h"

void UWildWestGameInstance::SetupServer(ECharacterState NewServerState)
{
	ServerCharacterState = NewServerState;
	
	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();
	if (LobbyGameState)
	{
		LobbyGameState->SetServerCharacterState(NewServerState);
	}
}

void UWildWestGameInstance::SetupClient(ECharacterState NewClientState)
{
	ClientCharacterState = NewClientState;

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->SetClientCharacterState(NewClientState);
		}
	}
}

void UWildWestGameInstance::CheckSelectedCharacter()
{
	if (ServerCharacterState == ECharacterState::ECS_None || ClientCharacterState == ECharacterState::ECS_None || ServerCharacterState == ClientCharacterState) return;
	
	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameMode* LobbyGameMode = World->GetAuthGameMode<ALobbyGameMode>();
		LobbyGameMode->TravelToTown();
	}
}

void UWildWestGameInstance::ReplaceVaultList(FVector Key, bool Value)
{
	VaultList[Key] = Value;
}
