// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Game/TownGameState.h"
#include "Player/TownPlayerController.h"
#include "Character/Gunman.h"
#include "Character/Sheriff.h"
#include "Game/WildWestGameInstance.h"
#include "Interactable/Vault.h"

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

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (WildWestGameInstance->LastTransformList.IsEmpty())
	{
		AActor* GunmanStart = UGameplayStatics::GetActorOfClass(this, GunmanStartClass);
		if (GunmanStart)
		{
			Gunman = GetWorld()->SpawnActor<AGunman>(GunmanClass, GunmanStart->GetActorTransform());
		}

		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		for (int32 Index = 0; Index < PlayerStarts.Num(); ++Index)
		{
			ASheriff* Sheriff = GetWorld()->SpawnActor<ASheriff>(SheriffClass, PlayerStarts[Index]->GetActorTransform());
			SheriffList.Add(Sheriff);
			Sheriff->SetSheriffIndex(Index);
		}
	}
	else
	{
		Gunman = GetWorld()->SpawnActor<AGunman>(GunmanClass, WildWestGameInstance->LastGunmanTransform);
		for (int32 Index = 0; Index < WildWestGameInstance->SheriffDeathList.Num(); ++Index)
		{
			if (WildWestGameInstance->SheriffDeathList[Index])
			{
				SheriffList.Add(nullptr);
			}
			else
			{
				ASheriff* Sheriff = GetWorld()->SpawnActor<ASheriff>(SheriffClass, WildWestGameInstance->LastTransformList[Index]);
				SheriffList.Add(Sheriff);
				Sheriff->SetSheriffIndex(Index);
			}
		}
	}
}

void ATownGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ATownGameState* TownGameState = Cast<ATownGameState>(GameState);
	if (TownGameState == nullptr) return;

	if (TownGameState->SheriffList.IsEmpty())
	{
		TownGameState->Gunman = Gunman;
		for (ASheriff* Sheriff : SheriffList)
		{
			TownGameState->SheriffList.Add(Sheriff);
		}
	}
	
	ATownPlayerController* TownPlayerController = Cast<ATownPlayerController>(NewPlayer);
	if (TownPlayerController)
	{
		TownPlayerController->InitialPossess();
	}
}

void ATownGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (WildWestGameInstance->IsGameStarted())
	{
		for (auto VaultTransform : WildWestGameInstance->GetVaultTransformList())
		{
			AVault* Vault = GetWorld()->SpawnActor<AVault>(VaultClass, VaultTransform);
			if (Vault)
			{
				TMap<FVector, bool> VaultList = WildWestGameInstance->GetVaultList();
				Vault->Open(VaultList[VaultTransform.GetLocation()]);
				if (VaultList[VaultTransform.GetLocation()])
				{
					Vault->OpenDoor();
				}
			}
		}
	}
	else
	{
		TArray<AActor*> VaultCaseList;
		UGameplayStatics::GetAllActorsOfClass(this, VaultCaseClass, VaultCaseList);
		for (int32 Index = 0; Index < NumberOfVault; ++Index)
		{
			AActor* VaultCase = VaultCaseList[FMath::RandRange(0, VaultCaseList.Num())];
			if (VaultCase)
			{
				FTransform VaultTransform = VaultCase->GetTransform();
				GetWorld()->SpawnActor<AActor>(VaultClass, VaultTransform);
				VaultCaseList.Remove(VaultCase);
				WildWestGameInstance->GetVaultTransformList().Add(VaultTransform);
				WildWestGameInstance->GetVaultList().Add(VaultTransform.GetLocation(), false);
				WildWestGameInstance->StartGame(true);
			}
		}
	}
}
