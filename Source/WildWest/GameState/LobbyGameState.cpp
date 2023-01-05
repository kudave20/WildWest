// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "WildWest/GameMode/LobbyGameMode.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/Controller/LobbyPlayerController.h"
#include "WildWest/Character/Gunman.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bIsLobbyFull);
}

void ALobbyGameState::GunmanButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (!HasAuthority())
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(-50.f, -604.f, 77.3f));
			SetupSpawnCharacter(ECharacterState::ECS_Gunman, SpawnTransform);

			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
			if (LobbyPlayerController)
			{
				LobbyPlayerController->ServerGunmanButtonClicked();
			}

			return;
		}

		if (bIsSheriffSelected)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(-50.f, -604.f, 77.3f));
			SetupSpawnCharacter(ECharacterState::ECS_Gunman, SpawnTransform);

			bIsGunmanSelected = true;
			CheckSelectedCharacter();
			return;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(-50.f, -604.f, 77.3f));
		SetupSpawnCharacter(ECharacterState::ECS_Gunman, SpawnTransform);

		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (LobbyPlayerController)
		{
			if (LobbyPlayerController->GetbIsSelectingCharacter()) bIsSheriffSelected = false;
			bIsGunmanSelected = true;
			LobbyPlayerController->SetbIsSelectingCharacter(true);
		}
	}
}

void ALobbyGameState::SheriffButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (!HasAuthority())
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(-50.f, -144.f, 77.3f));
			SetupSpawnCharacter(ECharacterState::ECS_Sheriff, SpawnTransform);

			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
			if (LobbyPlayerController)
			{
				LobbyPlayerController->ServerSheriffButtonClicked();
			}

			return;
		}

		if (bIsGunmanSelected)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FVector(-50.f, -144.f, 77.3f));
			SetupSpawnCharacter(ECharacterState::ECS_Sheriff, SpawnTransform);

			bIsSheriffSelected = true;
			CheckSelectedCharacter();
			return;
		}

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FVector(-50.f, -144.f, 77.3f));
		SetupSpawnCharacter(ECharacterState::ECS_Sheriff, SpawnTransform);

		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (LobbyPlayerController)
		{
			if (LobbyPlayerController->GetbIsSelectingCharacter()) bIsGunmanSelected = false;
			bIsSheriffSelected = true;
			LobbyPlayerController->SetbIsSelectingCharacter(true);
		}
	}
}

void ALobbyGameState::ServerGunmanButtonClicked()
{
	if (bIsSheriffSelected)
	{
		bIsGunmanSelected = true;
		CheckSelectedCharacter();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (LobbyPlayerController)
		{
			if (LobbyPlayerController->GetbIsSelectingCharacter()) bIsSheriffSelected = false;
			bIsGunmanSelected = true;
			LobbyPlayerController->SetbIsSelectingCharacter(true);
		}
	}
}

void ALobbyGameState::ServerSheriffButtonClicked()
{
	if (bIsGunmanSelected)
	{
		bIsSheriffSelected = true;
		CheckSelectedCharacter();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (LobbyPlayerController)
		{
			if (LobbyPlayerController->GetbIsSelectingCharacter()) bIsGunmanSelected = false;
			bIsSheriffSelected = true;
			LobbyPlayerController->SetbIsSelectingCharacter(true);
		}
	}
}

void ALobbyGameState::OnRep_bIsLobbyFull()
{
	if (bIsLobbyFull)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
			if (LobbyPlayerController)
			{
				LobbyPlayerController->SelectCharacterCompleteDelegate.Broadcast();
			}
		}
	}
}

void ALobbyGameState::CheckSelectedCharacter()
{
	if (bIsGunmanSelected && bIsSheriffSelected)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ALobbyGameMode* LobbyGameMode = World->GetAuthGameMode<ALobbyGameMode>();
			LobbyGameMode->TravelToTown();
		}
	}
}

void ALobbyGameState::SetupSpawnCharacter(ECharacterState CharacterState, FTransform Transform)
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupSpawn(CharacterState, Transform);
	}
}

void ALobbyGameState::SetbIsLobbyFull(bool bIsFull)
{
	bIsLobbyFull = bIsFull;

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
		if (LobbyPlayerController)
		{
			LobbyPlayerController->SelectCharacterCompleteDelegate.Broadcast();
		}
	}
}
