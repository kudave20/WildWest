// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "WildWest/HUD/ReturnToMainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/Character/Sheriff.h"
#include "WildWest/HUD/SheriffHUD.h"
#include "WildWest/HUD/CharacterOverlay.h"
#include "Components/TextBlock.h"
#include "WildWest/HUD/GameViewportWidget.h"
#include "Components/Image.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

void ATownPlayerController::SetInitialControlRotation(const FRotator& NewRotation)
{
	SetControlRotation(NewRotation);
	ClientSetRotation(NewRotation);
}

void ATownPlayerController::ClientRemovePlayer_Implementation()
{
	UGameplayStatics::RemovePlayer(this, true);
}

void ATownPlayerController::ServerNotifyDisconnected_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ATownPlayerController* TownPlayerController = Cast<ATownPlayerController>(World->GetFirstPlayerController());
		if (TownPlayerController)
		{
			TownPlayerController->NotifyDelegate.Broadcast();
		}
	}
}

/*void ATownPlayerController::ClientDestroySession_Implementation()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->DestroySession();
		}
	}
}*/

void ATownPlayerController::InitialPossess()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
	if (WildWestGameInstance == nullptr || TownGameState == nullptr) return;

	if (IsLocalPlayerController())
	{
		if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Gunman)
		{

		}
		else if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Sheriff)
		{
			TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();
			Possess(SheriffList[0]);
		}
	}
	else
	{
		if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Gunman)
		{

		}
		else if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Sheriff)
		{
			TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();
			Possess(SheriffList[0]);
		}
	}
}

void ATownPlayerController::SetSheriffHUDScreen(int32 ScreenIndex)
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid = SheriffHUD &&
		SheriffHUD->CharacterOverlay &&
		SheriffHUD->CharacterOverlay->ScreenIndex;
	if (bHUDValid)
	{
		FString ScreenIndexText = FString::Printf(TEXT("%d"), ScreenIndex);
		SheriffHUD->CharacterOverlay->ScreenIndex->SetText(FText::FromString(ScreenIndexText));
	}
}

void ATownPlayerController::SetSheriffHUDViewport(EScreenIndex ScreenIndex)
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid = SheriffHUD &&
		SheriffHUD->CharacterOverlay &&
		SheriffHUD->CharacterOverlay->FirstViewport &&
		SheriffHUD->CharacterOverlay->SecondViewport &&
		SheriffHUD->CharacterOverlay->ThirdViewport &&
		SheriffHUD->CharacterOverlay->FourthViewport;
	if (bHUDValid)
	{
		switch (ScreenIndex)
		{
		case EScreenIndex::ECI_First:
			SheriffHUD->CharacterOverlay->FirstViewport->SetVisibility(ESlateVisibility::Hidden);
			SheriffHUD->CharacterOverlay->SecondViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->FourthViewport->SetVisibility(ESlateVisibility::Visible);
			break;
		case EScreenIndex::ECI_Second:
			SheriffHUD->CharacterOverlay->FirstViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->SecondViewport->SetVisibility(ESlateVisibility::Hidden);
			SheriffHUD->CharacterOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->FourthViewport->SetVisibility(ESlateVisibility::Visible);
			break;
		case EScreenIndex::ECI_Third:
			SheriffHUD->CharacterOverlay->FirstViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->SecondViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Hidden);
			SheriffHUD->CharacterOverlay->FourthViewport->SetVisibility(ESlateVisibility::Visible);
			break;
		case EScreenIndex::ECI_Fourth:
			SheriffHUD->CharacterOverlay->FirstViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->SecondViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Visible);
			SheriffHUD->CharacterOverlay->FourthViewport->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
	}
}

void ATownPlayerController::SetSheriffHUDGauge(float GaugePercent)
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid = SheriffHUD &&
		SheriffHUD->CharacterOverlay &&
		SheriffHUD->CharacterOverlay->Gauge;
	if (bHUDValid)
	{
		UMaterialInstanceDynamic* GaugeMaterialInstance = SheriffHUD->CharacterOverlay->Gauge->GetDynamicMaterial();
		if (GaugeMaterialInstance)
		{
			GaugeMaterialInstance->SetScalarParameterValue(FName("GaugePercent"), GaugePercent);
		}
	}
}

void ATownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	SetShowMouseCursor(false);

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (Cast<ASheriff>(GetPawn()) != nullptr)
	{
		ClientSetHUD(SheriffHUDClass);
		SetSheriffHUDScreen(1);
		SetSheriffHUDViewport(EScreenIndex::ECI_First);
		SetCurrentScreenIndex(EScreenIndex::ECI_First);
	}
}

void ATownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMapping, 0);

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(InputActions->InputOption, ETriggerEvent::Triggered, this, &ATownPlayerController::ShowReturnToMainMenu);
		}
	}
}

void ATownPlayerController::ShowReturnToMainMenu()
{
	if (ReturnToMainMenuWidget == nullptr) return;
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuWidget);
	}
	if (ReturnToMainMenu)
	{
		bReturnToMainMenuOpen = !bReturnToMainMenuOpen;
		if (bReturnToMainMenuOpen)
		{
			ReturnToMainMenu->MenuSetup();
		}
		else
		{
			ReturnToMainMenu->MenuTearDown();
		}
	}
}
