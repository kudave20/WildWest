// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWest/Character/Sheriff.h"
#include "EngineUtils.h"

void ATownPlayerController::ServerCharacterSpawn_Implementation(TSubclassOf<AActor> SelectedCharacter, ECharacterState CharacterState)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = FVector(-80, 0, 72);
		FRotator Rotation = FRotator();

		switch (CharacterState)
		{
		case ECharacterState::ECS_Gunman:
			Location.Y = -604;
			SpawnedCharacter = World->SpawnActor<APawn>(SelectedCharacter, Location, Rotation);
			if (SpawnedCharacter)
			{
				ASheriff* Sheriff = Cast<ASheriff>(SpawnedCharacter);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
				}

				Possess(SpawnedCharacter);
			}

			break;
		case ECharacterState::ECS_Sheriff:
			Location.Y = -144;
			SpawnedCharacter = World->SpawnActor<APawn>(SelectedCharacter, Location, Rotation);
			if (SpawnedCharacter)
			{
				ASheriff* Sheriff = Cast<ASheriff>(SpawnedCharacter);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
				}

				Possess(SpawnedCharacter);
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
