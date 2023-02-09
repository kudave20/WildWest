// Fill out your copyright notice in the Description page of Project Settings.


#include "ReturnToLobby.h"
#include "Components/Button.h"
#include "WildWest/GameMode/DuelGameMode.h"
#include "WildWest/GameState/DuelGameState.h"
#include "WildWest/Controller/DuelPlayerController.h"

void UReturnToLobby::ReturnToLobbySetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			ADuelPlayerController* DuelPlayerController = NULL;
			if (World->IsNetMode(ENetMode::NM_ListenServer))
			{
				DuelPlayerController = DuelGameState->GetServerPlayerController();
			}
			else
			{
				DuelPlayerController = DuelGameState->GetClientPlayerController();
			}

			if (DuelPlayerController)
			{
				FInputModeUIOnly InputModeData;
				InputModeData.SetWidgetToFocus(TakeWidget());
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				DuelPlayerController->SetInputMode(InputModeData);
				DuelPlayerController->SetShowMouseCursor(true);
			}
		}
	}
}

bool UReturnToLobby::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &ThisClass::ReturnButtonClicked);
	}

	return true;
}

void UReturnToLobby::ReturnButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameMode* DuelGameMode = World->GetAuthGameMode<ADuelGameMode>();
		if (DuelGameMode)
		{
			DuelGameMode->TravelToLobby();
		}
	}
}
