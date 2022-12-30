// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunman.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WildWest/HUD/CharacterSelect.h"
#include "Net/UnrealNetwork.h"
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

void AGunman::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGunman, bIsLobbyFull);
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

void AGunman::OnRep_bIsLobbyFull()
{
	if (bIsLobbyFull)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerController* PlayerController = Cast<APlayerController>(World->GetFirstPlayerController());
			if (PlayerController)
			{
				CharacterSelect = CreateWidget<UCharacterSelect>(PlayerController, CharacterSelectWidget);
				if (CharacterSelect)
				{
					CharacterSelect->CharacterSelectSetup();
				}
			}
		}
	}
}

void AGunman::SetbIsLobbyFull(bool bIsFull)
{
	bIsLobbyFull = bIsFull;

	if (bIsLobbyFull && IsLocallyControlled())
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			CharacterSelect = CreateWidget<UCharacterSelect>(PlayerController, CharacterSelectWidget);
			if (CharacterSelect)
			{
				CharacterSelect->CharacterSelectSetup();
			}
		}
	}
}
