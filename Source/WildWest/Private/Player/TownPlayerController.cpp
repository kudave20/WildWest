// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TownPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HUD/ReturnToMainMenu.h"
#include "Game/WildWestGameInstance.h"
#include "Game/TownGameState.h"
#include "Character/Gunman.h"
#include "Character/Sheriff.h"
#include "Components/Border.h"
#include "HUD/SheriffHUD.h"
#include "HUD/SheriffOverlay.h"
#include "HUD/GameViewportWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HUD/VaultGauge.h"
#include "Interactable/Vault.h"
#include "WildWestTypes/ScreenIndex.h"

ATownPlayerController::ATownPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(TownContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(TownContext, 0);
	}

	bShowMouseCursor = false;
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
}

void ATownPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LastDetectedTime > 0)
	{
		LastDetectedTime -= DeltaSeconds;
	}
}

void ATownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATownPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATownPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATownPlayerController::Jump);
	EnhancedInputComponent->BindAction(OptionAction, ETriggerEvent::Started, this, &ATownPlayerController::ShowReturnToMainMenu);
}

void ATownPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (!bInputEnabled) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ATownPlayerController::Look(const FInputActionValue& InputActionValue)
{
	if (!bInputEnabled) return;
	
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	AddPitchInput(LookAxisVector.Y);
	AddYawInput(LookAxisVector.X);
}

void ATownPlayerController::Jump()
{
	if (!bInputEnabled) return;
	
	if (ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Jump();
	}
}

void ATownPlayerController::ShowReturnToMainMenu()
{
	if (ReturnToMainMenuClass == nullptr) return;
	if (ReturnToMainMenu == nullptr)
	{
		ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuClass);
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

void ATownPlayerController::ServerNotifyDisconnected_Implementation()
{
	if (NotifyClass)
	{
		UUserWidget* Notify = CreateWidget(this, NotifyClass);
		if (Notify)
		{
			Notify->AddToViewport();
		}
	}
	if (ReturnToMainMenuClass)
	{
		if (ReturnToMainMenu == nullptr)
		{
			ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuClass);
		}
		if (ReturnToMainMenu)
		{
			ReturnToMainMenu->MenuSetup();
		}
	}
}

void ATownPlayerController::ClientCreateGunmanVictory_Implementation()
{
	if (GunmanVictoryClass)
	{
		UUserWidget* GunmanVictory = CreateWidget(this, GunmanVictoryClass);
		if (GunmanVictory)
		{
			GunmanVictory->AddToViewport();
		}
	}
	if (ReturnToMainMenuClass)
	{
		if (ReturnToMainMenu == nullptr)
		{
			ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuClass);
		}
		if (ReturnToMainMenu)
		{
			ReturnToMainMenu->MenuSetup();
		}
	}
}

void ATownPlayerController::InitialPossess()
{
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
			PossessSheriff();
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
			PossessSheriff();
		}
	}
}

void ATownPlayerController::PossessGunman()
{
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr) return;

	AGunman* Gunman = TownGameState->Gunman;
	if (Gunman)
	{
		Possess(Gunman);
		SetControlRotation(Gunman->GetActorRotation());
		ClientSetRotation(Gunman->GetActorRotation());
	}
}

void ATownPlayerController::PossessSheriff()
{
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState)
	{
		Possess(TownGameState->SheriffList[0]);
		SetControlRotation(TownGameState->SheriffList[0]->GetActorRotation());
		ClientSetRotation(TownGameState->SheriffList[0]->GetActorRotation());
	}
}

void ATownPlayerController::SetGunmanPressWidget(AVault* TargetVault, TSubclassOf<UUserWidget> PressClass, bool bIsInteracting)
{
	if (Press == nullptr)
	{
		Press = CreateWidget(this, PressClass);
	}
	if (Press == nullptr) return;
	
	if (TargetVault == nullptr || bIsInteracting || TargetVault->IsOpened())
	{
		Press->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Press->SetVisibility(ESlateVisibility::Visible);
	}
}

void ATownPlayerController::SetGunmanVaultGauge(AVault* TargetVault, TSubclassOf<UUserWidget> VaultGaugeClass, bool bIsInteracting)
{
	if (VaultGaugeClass == nullptr)
	{
		VaultGauge = CreateWidget<UVaultGauge>(this, VaultGaugeClass);
	}
	if (VaultGauge == nullptr) return;

	if (TargetVault == nullptr || !bIsInteracting || TargetVault->IsOpened())
	{
		VaultGauge->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		VaultGauge->SetVisibility(ESlateVisibility::Visible);
		
		UImage* Gauge = VaultGauge->GetGauge();
		if (Gauge)
		{
			UMaterialInstanceDynamic* GaugeMaterialInstance = Gauge->GetDynamicMaterial();
			if (GaugeMaterialInstance)
			{
				GaugeMaterialInstance->SetScalarParameterValue(FName("GaugePercent"), TargetVault->GetOpenTimer() / TargetVault->GetMaxOpenTimer());
			}
		}
	}
}

void ATownPlayerController::SheriffHUDSetup()
{
	ClientSetHUD(SheriffHUDClass);
	SheriffHUD = Cast<ASheriffHUD>(GetHUD());
	if (SheriffHUD)
	{
		SheriffHUD->AddSheriffOverlay();
		CurrentScreenIndex = EScreenIndex::ECI_First;
		SetSheriffHUDScreen(1);
		HideSheriffHUDViewport();
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

void ATownPlayerController::SetSheriffHUDViewport(int32 SheriffIndex)
{
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr || TownGameState->SheriffList.Num() < 4 || TownGameState->SheriffList[SheriffIndex] == nullptr) return;
	
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	
	bool bHUDValid =
		SheriffHUD &&
		SheriffHUD->SheriffOverlay &&
		SheriffHUD->SheriffOverlay->Viewport &&
		SheriffHUD->SheriffOverlay->Border &&
		SheriffHUD->SheriffOverlay->ViewportIndex;
	
	if (!bHUDValid) return;
	
	FVector Location;
	FRotator Rotation;
	TownGameState->SheriffList[SheriffIndex]->GetActorEyesViewPoint(Location, Rotation);
		
	Location += Rotation.Vector() * 10.f;
	SheriffHUD->SheriffOverlay->Viewport->SetCameraLocation(false, Location);
	SheriffHUD->SheriffOverlay->Viewport->SetCameraRotation(false, Rotation);
	SheriffHUD->SheriffOverlay->Viewport->SetVisibility(ESlateVisibility::Visible);
	SheriffHUD->SheriffOverlay->Border->SetVisibility(ESlateVisibility::Visible);
	
	FString SheriffIndexText = FString::Printf(TEXT("%d"), SheriffIndex + 1);
	SheriffHUD->SheriffOverlay->ViewportIndex->SetText(FText::FromString(SheriffIndexText));
	SheriffHUD->SheriffOverlay->ViewportIndex->SetVisibility(ESlateVisibility::Visible);
}

void ATownPlayerController::HideSheriffHUDViewport()
{
	SheriffHUD = SheriffHUD == nullptr ? Cast<ASheriffHUD>(GetHUD()) : SheriffHUD;
	bool bHUDValid =
		SheriffHUD &&
		SheriffHUD->SheriffOverlay &&
		SheriffHUD->SheriffOverlay->Viewport &&
		SheriffHUD->SheriffOverlay->Border &&
		SheriffHUD->SheriffOverlay->ViewportIndex;
	
	if (!bHUDValid) return;
	
	SheriffHUD->SheriffOverlay->Viewport->SetVisibility(ESlateVisibility::Hidden);
	SheriffHUD->SheriffOverlay->Border->SetVisibility(ESlateVisibility::Hidden);
	SheriffHUD->SheriffOverlay->ViewportIndex->SetVisibility(ESlateVisibility::Hidden);
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

void ATownPlayerController::StartStun()
{
	DisableInput(this);

	Stun = CreateWidget(this, StunClass);
	if (Stun)
	{
		Stun->AddToViewport();
	}
}

void ATownPlayerController::Detect(ASheriff* InDetector, float InLastDetectedTime)
{
	Detector = InDetector;
	LastDetectedTime = InLastDetectedTime;
}
