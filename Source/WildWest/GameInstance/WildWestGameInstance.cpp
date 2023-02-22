// Fill out your copyright notice in the Description page of Project Settings.


#include "WildWestGameInstance.h"
#include "WildWest/Controller/LobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UWildWestGameInstance::SetupServer(ECharacterState NewServerState)
{
	ServerCharacterState = NewServerState;

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(UGameplayStatics::GetPlayerController(World, 1));
		if (LobbyPlayerController)
		{
			LobbyPlayerController->ClientBroadcastServerCharacter(NewServerState);
		}
	}
}

void UWildWestGameInstance::SetupClient(ECharacterState NewClientState)
{
	ClientCharacterState = NewClientState;

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (LobbyPlayerController)
		{
			LobbyPlayerController->SelectClientCharacterDelegate.Broadcast(NewClientState);
		}
	}
}

void UWildWestGameInstance::AddLastTransformList(FTransform LastTransform)
{
	LastTransformList.Add(LastTransform);
}

void UWildWestGameInstance::ReplaceVaultList(FVector Key, bool Value)
{
	VaultList[Key] = Value;
}
