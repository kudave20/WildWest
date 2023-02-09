// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect.h"
#include "Components/Button.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWest/Controller/LobbyPlayerController.h"

void UCharacterSelect::CharacterSelectSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			ALobbyPlayerController* LobbyPlayerController = NULL;
			if (World->IsNetMode(ENetMode::NM_ListenServer))
			{
				LobbyPlayerController = LobbyGameState->GetServerPlayerController();
			}
			else
			{
				LobbyPlayerController = LobbyGameState->GetClientPlayerController();
			}

			if (LobbyPlayerController)
			{
				FInputModeUIOnly InputModeData;
				InputModeData.SetWidgetToFocus(TakeWidget());
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				LobbyPlayerController->SetInputMode(InputModeData);
				LobbyPlayerController->SetShowMouseCursor(true);
			}
		}
	}
}

bool UCharacterSelect::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (GunmanButton)
	{
		GunmanButton->OnClicked.AddDynamic(this, &ThisClass::GunmanButtonClicked);
	}
	if (SheriffButton)
	{
		SheriffButton->OnClicked.AddDynamic(this, &ThisClass::SheriffButtonClicked);
	}

	return true;
}

void UCharacterSelect::GunmanButtonClicked()
{
	GunmanButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->GunmanButtonClicked();
		}
	}

	SheriffButton->SetIsEnabled(true);
}

void UCharacterSelect::SheriffButtonClicked()
{
	SheriffButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->SheriffButtonClicked();
		}
	}

	GunmanButton->SetIsEnabled(true);
}
