// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWest/Character/Gunman.h"
#include "WildWest/Character/Sheriff.h"
#include "EngineUtils.h"

void ATownPlayerController::ServerCharacterSpawn_Implementation(TSubclassOf<AActor> SelectedCharacter, ECharacterState CharacterState)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = FVector(-80, 0, 72);
		FRotator Rotation = FRotator();
		AGunman* Gunman = nullptr;
		ASheriff* Sheriff = nullptr;

		switch (CharacterState)
		{
		case ECharacterState::ECS_Gunman:
			Location.Y = -604;
			Gunman = World->SpawnActor<AGunman>(SelectedCharacter, Location, Rotation);
			if (Gunman)
			{
				Possess(Gunman);
			}

			break;
		case ECharacterState::ECS_Sheriff:
			Location.Y = -144;
			Sheriff = World->SpawnActor<ASheriff>(SelectedCharacter, Location, Rotation);
			if (Sheriff)
			{
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
				}

				Possess(Sheriff);
			}

			break;
		}
	}
}

void ATownPlayerController::ServerSheriffListSetup_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (ASheriff* Sheriff : TActorRange<ASheriff>(World))
		{
			SheriffList.Add(Sheriff);
		}

		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			TownGameState->SetSheriffList(SheriffList);
		}
	}
}
