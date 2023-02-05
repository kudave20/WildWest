// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameMode.h"

void ADuelGameMode::TravelToLobby()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
	}
}