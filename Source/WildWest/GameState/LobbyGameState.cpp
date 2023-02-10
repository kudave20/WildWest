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

		SetupServer(ECharacterState::ECS_Gunman);

		if (bIsSheriffSelected)
		{
			bIsGunmanSelected = true;
			CheckSelectedCharacter();
			return;
		}

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

		SetupServer(ECharacterState::ECS_Sheriff);

		if (bIsGunmanSelected)
		{
			bIsSheriffSelected = true;
			CheckSelectedCharacter();
			return;
		}

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
	SetupClient(ECharacterState::ECS_Gunman);

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
	SetupClient(ECharacterState::ECS_Sheriff);

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
				LobbyPlayerController->LobbyFullDelegate.Broadcast();
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
			LobbyPlayerController->LobbyFullDelegate.Broadcast();
		}
	}
}
