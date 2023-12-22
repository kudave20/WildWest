// Fill out your copyright notice in the Description page of Project Settings.


#include "ReturnToMainMenu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "GameFramework/GameModeBase.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/Controller/TownPlayerController.h"
#include "WildWest/Controller/DuelPlayerController.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

void UReturnToMainMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	if (ReturnButton && !ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.AddDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &UReturnToMainMenu::OnDestroySession);
		}
	}
}

void UReturnToMainMenu::MenuTearDown()
{
	RemoveFromParent();

	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	if (ReturnButton && ReturnButton->OnClicked.IsBound())
	{
		ReturnButton->OnClicked.RemoveDynamic(this, &UReturnToMainMenu::ReturnButtonClicked);
	}

	if (MultiplayerSessionsSubsystem && !MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.IsBound())
	{
		MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.RemoveDynamic(this, &UReturnToMainMenu::OnDestroySession);
	}
}

bool UReturnToMainMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void UReturnToMainMenu::OnDestroySession(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ReturnButton->SetIsEnabled(true);
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			WildWestGameInstance->StartGame(false);
			WildWestGameInstance->ChaseLastly(false);
			WildWestGameInstance->SetServerCharacterState(ECharacterState::ECS_None);
			WildWestGameInstance->SetClientCharacterState(ECharacterState::ECS_None);
			WildWestGameInstance->GetLastTransformList().Empty();
			WildWestGameInstance->SetLastGunmanTransform(FTransform());
			WildWestGameInstance->GetVaultList().Empty();
			WildWestGameInstance->GetVaultTransformList().Empty();
			WildWestGameInstance->SetVaultOpened(0);

			AGameModeBase* GameMode = World->GetAuthGameMode<AGameModeBase>();
			if (GameMode)
			{
				GameMode->ReturnToMainMenuHost();
			}
			else
			{
				PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
				ATownPlayerController* TownPlayerController = Cast<ATownPlayerController>(PlayerController);
				if (TownPlayerController)
				{
					TownPlayerController->ServerNotifyDisconnected();
					TownPlayerController->ClientReturnToMainMenuWithTextReason(FText());
				}
			}
		}
	}
}

void UReturnToMainMenu::ReturnButtonClicked()
{
	ReturnButton->SetIsEnabled(false);

	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->DestroySession();
	}
}
