// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "WildWest/HUD/CharacterSelect.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/Character/Gunman.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AGunman* Gunman = Cast<AGunman>(UGameplayStatics::GetPlayerPawn(World, 0));
			if (Gunman)
			{
				Gunman->SetbIsLobbyFull(true);
			}
		}
	}
}