// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "WildWest/GameMode/LobbyGameMode.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/Controller/LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, ClientPlayerController);
}

void ALobbyGameState::GunmanButtonClicked()
{
	if (!HasAuthority())
	{
		if (ClientPlayerController)
		{
			ClientPlayerController->ServerGunmanButtonClicked();
		}

		return;
	}

	SetupServer(ECharacterState::ECS_Gunman);

	if (ClientPlayerController && ClientPlayerController->GetbIsSelectingCharacter() && bIsSheriffSelected)
	{
		bIsGunmanSelected = true;
		CheckSelectedCharacter();
		return;
	}

	if (ServerPlayerController)
	{
		if (ServerPlayerController->GetbIsSelectingCharacter()) bIsSheriffSelected = false;
		bIsGunmanSelected = true;
		ServerPlayerController->SetbIsSelectingCharacter(true);
	}
}

void ALobbyGameState::SheriffButtonClicked()
{
	if (!HasAuthority())
	{
		if (ClientPlayerController)
		{
			ClientPlayerController->ServerSheriffButtonClicked();
		}

		return;
	}

	SetupServer(ECharacterState::ECS_Sheriff);

	if (ClientPlayerController && ClientPlayerController->GetbIsSelectingCharacter() && bIsGunmanSelected)
	{
		bIsSheriffSelected = true;
		CheckSelectedCharacter();
		return;
	}

	if (ServerPlayerController)
	{
		if (ServerPlayerController->GetbIsSelectingCharacter()) bIsGunmanSelected = false;
		bIsSheriffSelected = true;
		ServerPlayerController->SetbIsSelectingCharacter(true);
	}
}

void ALobbyGameState::ServerGunmanButtonClicked()
{
	SetupClient(ECharacterState::ECS_Gunman);

	if (ServerPlayerController && ServerPlayerController->GetbIsSelectingCharacter() && bIsSheriffSelected)
	{
		bIsGunmanSelected = true;
		CheckSelectedCharacter();
		return;
	}

	if (ClientPlayerController)
	{
		if (ClientPlayerController->GetbIsSelectingCharacter()) bIsSheriffSelected = false;
		bIsGunmanSelected = true;
		ClientPlayerController->SetbIsSelectingCharacter(true);
	}
}

void ALobbyGameState::ServerSheriffButtonClicked()
{
	SetupClient(ECharacterState::ECS_Sheriff);

	if (ServerPlayerController && ServerPlayerController->GetbIsSelectingCharacter() && bIsGunmanSelected)
	{
		bIsSheriffSelected = true;
		CheckSelectedCharacter();
		return;
	}

	if (ClientPlayerController)
	{
		if (ClientPlayerController->GetbIsSelectingCharacter()) bIsGunmanSelected = false;
		bIsSheriffSelected = true;
		ClientPlayerController->SetbIsSelectingCharacter(true);
	}
}

void ALobbyGameState::WidgetSetup()
{
	if (ServerPlayerController)
	{
		ServerPlayerController->LobbyFullDelegate.Broadcast();
	}
}

void ALobbyGameState::SetServerPlayerController(APlayerController* NewPlayer)
{
	ServerPlayerController = Cast<ALobbyPlayerController>(NewPlayer);
}

void ALobbyGameState::SetClientPlayerController(APlayerController* NewPlayer)
{
	ClientPlayerController = Cast<ALobbyPlayerController>(NewPlayer);
}

void ALobbyGameState::OnRep_ClientPlayerController()
{
	if (ClientPlayerController)
	{
		ClientPlayerController->LobbyFullDelegate.Broadcast();
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
