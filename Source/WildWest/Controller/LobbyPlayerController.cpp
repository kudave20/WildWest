// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWest/HUD/CharacterSelect.h"
#include "Components/Button.h"

void ALobbyPlayerController::GunmanButtonClicked()
{
	if (!HasAuthority())
	{
		ServerGunmanButtonClicked();
		return;
	}
	
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupServer(ECharacterState::ECS_Gunman);
		WildWestGameInstance->CheckSelectedCharacter();
	}
}

void ALobbyPlayerController::ServerGunmanButtonClicked_Implementation()
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupClient(ECharacterState::ECS_Gunman);
		WildWestGameInstance->CheckSelectedCharacter();
	}
}

void ALobbyPlayerController::SheriffButtonClicked()
{
	if (!HasAuthority())
	{
		ServerSheriffButtonClicked();
		return;
	}
	
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupServer(ECharacterState::ECS_Sheriff);
		WildWestGameInstance->CheckSelectedCharacter();
	}
}

void ALobbyPlayerController::ServerSheriffButtonClicked_Implementation()
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		WildWestGameInstance->SetupClient(ECharacterState::ECS_Sheriff);
		WildWestGameInstance->CheckSelectedCharacter();
	}
}

void ALobbyPlayerController::AddCharacterSelect()
{
	if (CharacterSelectClass)
	{
		CharacterSelect = CreateWidget<UCharacterSelect>(this, CharacterSelectClass);
		if (CharacterSelect)
		{
			CharacterSelect->CharacterSelectSetup();
		}
	}
}

void ALobbyPlayerController::ChangeButtonState(ECharacterState NewState)
{
	if (CharacterSelect == nullptr || CharacterSelect->GunmanButton == nullptr || CharacterSelect->SheriffButton == nullptr) return;
	
	switch (NewState)
	{
	case ECharacterState::ECS_Gunman:
		CharacterSelect->GunmanButton->SetIsEnabled(false);
		CharacterSelect->SheriffButton->SetIsEnabled(true);
		break;
	case ECharacterState::ECS_Sheriff:
		CharacterSelect->SheriffButton->SetIsEnabled(false);
		CharacterSelect->GunmanButton->SetIsEnabled(true);
		break;
	}
}
