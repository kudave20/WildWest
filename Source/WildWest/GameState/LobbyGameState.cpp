// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "WildWest/GameMode/LobbyGameMode.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/Controller/LobbyPlayerController.h"
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
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
			if (LobbyPlayerController)
			{
				LobbyPlayerController->ServerGunmanButtonClicked();
			}

			return;
		}

		if (bIsSheriffSelected)
		{
			SetupServer(ECharacterState::ECS_Gunman);

			bIsGunmanSelected = true;
			CheckSelectedCharacter();
			return;
		}

		SetupServer(ECharacterState::ECS_Gunman);

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
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
			if (LobbyPlayerController)
			{
				LobbyPlayerController->ServerSheriffButtonClicked();
			}

			return;
		}

		if (bIsGunmanSelected)
		{
			SetupServer(ECharacterState::ECS_Sheriff);

			bIsSheriffSelected = true;
			CheckSelectedCharacter();
			return;
		}

		SetupServer(ECharacterState::ECS_Sheriff);

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
		SetupClient(ECharacterState::ECS_Gunman);
		CheckSelectedCharacter();
		return;
	}

	SetupClient(ECharacterState::ECS_Gunman);

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
		SetupClient(ECharacterState::ECS_Sheriff);
		CheckSelectedCharacter();
		return;
	}

	SetupClient(ECharacterState::ECS_Sheriff);

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

void ALobbyGameState::SetupServer(ECharacterState NewServerState)
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupServer(NewServerState);
	}
}

void ALobbyGameState::SetupClient(ECharacterState NewClientState)
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupClient(NewClientState);
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
