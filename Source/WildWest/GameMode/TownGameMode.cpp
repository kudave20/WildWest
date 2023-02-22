// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameMode.h"

void ATownGameMode::TravelToDuel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Duel?listen"));
	}
}