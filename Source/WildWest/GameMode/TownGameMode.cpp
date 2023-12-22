// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/Controller/TownPlayerController.h"
#include "WildWest/Character/Gunman.h"
#include "WildWest/Character/Sheriff.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/Props/Vault.h"

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

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (WildWestGameInstance->GetLastTransformList().IsEmpty())
	{
		AActor* GunmanStart = UGameplayStatics::GetActorOfClass(this, GunmanStartClass);
		if (GunmanStart)
		{
			Gunman = World->SpawnActor<AGunman>(GunmanClass, GunmanStart->GetActorTransform());
		}

		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		for (int32 Index = 0; Index < PlayerStarts.Num(); ++Index)
		{
			ASheriff* Sheriff = World->SpawnActor<ASheriff>(SheriffClass, PlayerStarts[Index]->GetActorTransform());
			SheriffList.Add(Sheriff);
			if (Sheriff)
			{
				Sheriff->SetSheriffIndex(static_cast<EScreenIndex>(Index));
			}
		}
	}
	else
	{
		Gunman = World->SpawnActor<AGunman>(GunmanClass, WildWestGameInstance->GetLastGunmanTransform());
		for (FTransform LastTransform : WildWestGameInstance->GetLastTransformList())
		{
			if (LastTransform.Equals(FTransform::Identity))
			{
				SheriffList.Add(nullptr);
			}
			else
			{
				ASheriff* Sheriff = World->SpawnActor<ASheriff>(SheriffClass, LastTransform);
				SheriffList.Add(Sheriff);
			}
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
		TownGameState->SetGunman(Gunman);
		TArray<ASheriff*>& TargetSheriffList = TownGameState->GetSheriffList();
		for (ASheriff* Sheriff : SheriffList)
		{
			TargetSheriffList.Add(Sheriff);
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
				Vault->SetbIsOpened(VaultList[VaultTransform.GetLocation()]);
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
