// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "WildWest/HUD/ReturnToMainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/Character/Gunman.h"
#include "WildWest/Character/Sheriff.h"
#include "WildWest/HUD/SheriffHUD.h"
#include "WildWest/HUD/SheriffOverlay.h"
#include "Components/TextBlock.h"
#include "WildWest/HUD/GameViewportWidget.h"
#include "Components/Image.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

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

void ATownPlayerController::InitialPossess()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (IsLocalPlayerController())
	{
		if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Gunman)
		{
			PossessGunman();
		}
		else if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Sheriff)
		{
			PossessSheriffRandomly();
		}
	}
	else
	{
		if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Gunman)
		{
			PossessGunman();
		}
		else if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Sheriff)
		{
			PossessSheriffRandomly();
		}
	}
}

void ATownPlayerController::PossessGunman()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr) return;

	AGunman* Gunman = TownGameState->GetGunman();
	if (Gunman)
	{
		Possess(Gunman);
		SetControlRotation(Gunman->GetActorRotation());
		ClientSetRotation(Gunman->GetActorRotation());
	}
}

void ATownPlayerController::PossessSheriffRandomly()
{
	UWorld* World = GetWorld();
	if (World)
	{
		TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
		if (TownGameState)
		{
			TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();
			Possess(SheriffList[0]);
			SetControlRotation(SheriffList[0]->GetActorRotation());
			ClientSetRotation(SheriffList[0]->GetActorRotation());
		}
	}
}

void ATownPlayerController::SheriffHUDSetup(APawn* InPawn)
{
	if (InPawn && InPawn->IsA(ASheriff::StaticClass()))
	{
		ClientSetHUD(SheriffHUDClass);
		SheriffHUD = Cast<ASheriffHUD>(GetHUD());
		if (SheriffHUD)
		{
			SheriffHUD->AddSheriffOverlay();
			SetSheriffHUDScreen(1);
			SetSheriffHUDViewport(EScreenIndex::ECI_First, EScreenIndex::ECI_First);
			SetCurrentScreenIndex(EScreenIndex::ECI_First);
		}
	}
}

void ATownPlayerController::SetSheriffHUDScreen(int32 ScreenIndex)
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid = SheriffHUD &&
		SheriffHUD->SheriffOverlay &&
		SheriffHUD->SheriffOverlay->ScreenIndex;
	if (bHUDValid)
	{
		FString ScreenIndexText = FString::Printf(TEXT("%d"), ScreenIndex);
		SheriffHUD->SheriffOverlay->ScreenIndex->SetText(FText::FromString(ScreenIndexText));
	}
}

void ATownPlayerController::SetSheriffHUDViewport(EScreenIndex ScreenIndex, EScreenIndex PreviousScreenIndex)
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid = SheriffHUD &&
		SheriffHUD->SheriffOverlay &&
		SheriffHUD->SheriffOverlay->FirstViewport &&
		SheriffHUD->SheriffOverlay->SecondViewport &&
		SheriffHUD->SheriffOverlay->ThirdViewport &&
		SheriffHUD->SheriffOverlay->FourthViewport;
	if (bHUDValid)
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
		if (TownGameState == nullptr) return;

		FVector Location;
		FRotator Rotation;

		switch (ScreenIndex)
		{
		case EScreenIndex::ECI_First:
			SheriffHUD->SheriffOverlay->FirstViewport->SetVisibility(ESlateVisibility::Hidden);

			switch (PreviousScreenIndex)
			{
			case EScreenIndex::ECI_First:
				break;
			case EScreenIndex::ECI_Second:
				SheriffHUD->SheriffOverlay->SecondViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[1]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->SecondViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->SecondViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Third:
				SheriffHUD->SheriffOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[2]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->ThirdViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->ThirdViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Fourth:
				SheriffHUD->SheriffOverlay->FourthViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[3]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->FourthViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->FourthViewport->SetCameraRotation(false, Rotation);
				break;
			}
			break;
		case EScreenIndex::ECI_Second:
			SheriffHUD->SheriffOverlay->SecondViewport->SetVisibility(ESlateVisibility::Hidden);

			switch (PreviousScreenIndex)
			{
			case EScreenIndex::ECI_First:
				SheriffHUD->SheriffOverlay->FirstViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[0]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->FirstViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->FirstViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Second:
				break;
			case EScreenIndex::ECI_Third:
				SheriffHUD->SheriffOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[2]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->ThirdViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->ThirdViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Fourth:
				SheriffHUD->SheriffOverlay->FourthViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[3]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->FourthViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->FourthViewport->SetCameraRotation(false, Rotation);
				break;
			}
			break;
		case EScreenIndex::ECI_Third:
			SheriffHUD->SheriffOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Hidden);

			switch (PreviousScreenIndex)
			{
			case EScreenIndex::ECI_First:
				SheriffHUD->SheriffOverlay->FirstViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[0]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->FirstViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->FirstViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Second:
				SheriffHUD->SheriffOverlay->SecondViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[1]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->SecondViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->SecondViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Third:
				break;
			case EScreenIndex::ECI_Fourth:
				SheriffHUD->SheriffOverlay->FourthViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[3]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->FourthViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->FourthViewport->SetCameraRotation(false, Rotation);
				break;
			}
			break;
		case EScreenIndex::ECI_Fourth:
			SheriffHUD->SheriffOverlay->FourthViewport->SetVisibility(ESlateVisibility::Hidden);

			switch (PreviousScreenIndex)
			{
			case EScreenIndex::ECI_First:
				SheriffHUD->SheriffOverlay->FirstViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[0]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->FirstViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->FirstViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Second:
				SheriffHUD->SheriffOverlay->SecondViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[1]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->SecondViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->SecondViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Third:
				SheriffHUD->SheriffOverlay->ThirdViewport->SetVisibility(ESlateVisibility::Visible);
				TownGameState->GetSheriffList()[2]->GetActorEyesViewPoint(Location, Rotation);
				Location += Rotation.Vector() * 10.f;
				SheriffHUD->SheriffOverlay->ThirdViewport->SetCameraLocation(false, Location);
				SheriffHUD->SheriffOverlay->ThirdViewport->SetCameraRotation(false, Rotation);
				break;
			case EScreenIndex::ECI_Fourth:
				break;
			}
			break;
		}
	}
}

void ATownPlayerController::SetSheriffHUDGauge(float GaugePercent)
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid = SheriffHUD &&
		SheriffHUD->SheriffOverlay &&
		SheriffHUD->SheriffOverlay->Gauge;
	if (bHUDValid)
	{
		UMaterialInstanceDynamic* GaugeMaterialInstance = SheriffHUD->SheriffOverlay->Gauge->GetDynamicMaterial();
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
