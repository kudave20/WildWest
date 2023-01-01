// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunman.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

AGunman::AGunman()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
}

void AGunman::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGunman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &AGunman::MoveForward);
	EnhancedInputComponent->BindAction(InputActions->InputMoveRight, ETriggerEvent::Triggered, this, &AGunman::MoveRight);
	EnhancedInputComponent->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &AGunman::Turn);
	EnhancedInputComponent->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &AGunman::LookUp);
}

void AGunman::GunmanButtonClicked()
{
	if (!HasAuthority())
	{
		ServerGunmanButtonClicked();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			if (LobbyGameState->GetbIsSheriffSelected())
			{
				LobbyGameState->SetbIsGunmanSelected(true);
				return;
			}

			if (bIsSelectingCharacter) LobbyGameState->SetbIsSheriffSelected(false);
			LobbyGameState->SetbIsGunmanSelected(true);
			bIsSelectingCharacter = true;
		}
	}
}

void AGunman::SheriffButtonClicked()
{
	if (!HasAuthority())
	{
		ServerSheriffButtonClicked();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			if (LobbyGameState->GetbIsGunmanSelected())
			{
				LobbyGameState->SetbIsSheriffSelected(true);
				return;
			}

			if (bIsSelectingCharacter) LobbyGameState->SetbIsGunmanSelected(false);
			LobbyGameState->SetbIsSheriffSelected(true);
			bIsSelectingCharacter = true;
		}
	}
}

void AGunman::ServerGunmanButtonClicked_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			if (LobbyGameState->GetbIsSheriffSelected())
			{
				LobbyGameState->SetbIsGunmanSelected(true);
				return;
			}

			if (bIsSelectingCharacter) LobbyGameState->SetbIsSheriffSelected(false);
			LobbyGameState->SetbIsGunmanSelected(true);
			bIsSelectingCharacter = true;
		}
	}
}

void AGunman::ServerSheriffButtonClicked_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			if (LobbyGameState->GetbIsGunmanSelected())
			{
				LobbyGameState->SetbIsSheriffSelected(true);
				return;
			}

			if (bIsSelectingCharacter) LobbyGameState->SetbIsGunmanSelected(false);
			LobbyGameState->SetbIsSheriffSelected(true);
			bIsSelectingCharacter = true;
		}
	}
}

void AGunman::MoveForward(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const float MoveValue = Value.Get<float>();

		if (MoveValue != 0.f)
		{
			const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
			AddMovementInput(Direction, MoveValue);
		}
	}
}

void AGunman::MoveRight(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const float MoveValue = Value.Get<float>();

		if (MoveValue != 0.f)
		{
			const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
			const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
			AddMovementInput(Direction, MoveValue);
		}
	}
}

void AGunman::Turn(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void AGunman::LookUp(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}
