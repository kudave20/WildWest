// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "WildWest/GameState/LobbyGameState.h"

void ATownPlayerController::ServerCharacterSpawn_Implementation(TSubclassOf<AActor> SelectedCharacter, ECharacterState CharacterState)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = FVector(-80, 0, 72);
		FRotator Rotation = FRotator();
		APawn* SpawnedCharacter;

		switch (CharacterState)
		{
		case ECharacterState::ECS_Gunman:
			Location.Y = -604;
			SpawnedCharacter = World->SpawnActor<APawn>(SelectedCharacter, Location, Rotation);
			if (SpawnedCharacter)
			{
				Possess(SpawnedCharacter);
			}

			break;
		case ECharacterState::ECS_Sheriff:
			Location.Y = -144;
			SpawnedCharacter = World->SpawnActor<APawn>(SelectedCharacter, Location, Rotation);
			if (SpawnedCharacter)
			{
				Possess(SpawnedCharacter);
			}

			break;
		}
	}
}