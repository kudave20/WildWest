// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/Controller/TownPlayerController.h"

void ATownGameMode::TravelToDuel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Duel?listen"));
	}
}

void ATownGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	for (AActor* PlayerStart : PlayerStarts)
	{
		ASheriff* Sheriff = World->SpawnActor<ASheriff>(SheriffClass, PlayerStart->GetActorTransform());
		if (Sheriff)
		{
			SheriffList.Add(Sheriff);
		}
	}
}

void ATownGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ATownGameState* TownGameState = Cast<ATownGameState>(GameState);
	if (TownGameState == nullptr) return;

	if (TownGameState->GetSheriffList().IsEmpty())
	{
		TownGameState->SetSheriffList(SheriffList);
	}

	ATownPlayerController* TownPlayerController = Cast<ATownPlayerController>(NewPlayer);
	if (TownPlayerController)
	{
		TownPlayerController->InitialPossess();
	}
}
