// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/LobbyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/WildWestGameInstance.h"
#include "HUD/CharacterSelect.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "HUD/ReturnToMainMenu.h"

void ALobbyPlayerController::BeginPlay()
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

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALobbyPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALobbyPlayerController::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ALobbyPlayerController::Jump);
	EnhancedInputComponent->BindAction(OptionAction, ETriggerEvent::Started, this, &ALobbyPlayerController::ShowReturnToMainMenu);
}

void ALobbyPlayerController::Move(const FInputActionValue& InputActionValue)
{
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

void ALobbyPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	
	AddPitchInput(LookAxisVector.Y);
	AddYawInput(LookAxisVector.X);
}

void ALobbyPlayerController::Jump()
{
	if (ACharacter* ControlledCharacter = GetPawn<ACharacter>())
	{
		ControlledCharacter->Jump();
	}
}

void ALobbyPlayerController::ShowReturnToMainMenu()
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
