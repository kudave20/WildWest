// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerStart.h"
#include "WildWest/Character/DuelGunman.h"
#include "WildWest/Character/DuelSheriff.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"

void ADuelGameMode::TravelToLobby()
{
	UWorld* World = GetWorld();
	if (World)
	{
		//bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
	}
}

void ADuelGameMode::TravelToTown()
{
	UWorld* World = GetWorld();
	if (World)
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Town?listen"));
	}
}

void ADuelGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	Camera = UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass());

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
	DuelGunman = World->SpawnActor<ADuelGunman>(DuelGunmanClass, PlayerStarts[0]->GetActorTransform());
	DuelSheriff = World->SpawnActor<ADuelSheriff>(DuelSheriffClass, PlayerStarts[1]->GetActorTransform());
}

void ADuelGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (NewPlayer)
	{
		if (NewPlayer->IsLocalPlayerController())
		{
			if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Gunman)
			{
				NewPlayer->Possess(DuelGunman);
			}
			else if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Sheriff)
			{
				NewPlayer->Possess(DuelSheriff);
			}
		}
		else
		{
			if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Gunman)
			{
				NewPlayer->Possess(DuelGunman);
			}
			else if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Sheriff)
			{
				NewPlayer->Possess(DuelSheriff);
			}
		}

		NewPlayer->SetViewTargetWithBlend(Camera);
	}
}
