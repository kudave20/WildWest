// Fill out your copyright notice in the Description page of Project Settings.


#include "Sheriff.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneCaptureComponent2D.h"
#include "WildWest/GameState/TownGameState.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

ASheriff::ASheriff()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;

	Screen = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Screen"));
	Screen->SetupAttachment(Camera);
}

void ASheriff::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			TownGameState->SheriffListSetup();
		}
	}
}

void ASheriff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASheriff::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &ASheriff::MoveForward);
	EnhancedInputComponent->BindAction(InputActions->InputMoveRight, ETriggerEvent::Triggered, this, &ASheriff::MoveRight);
	EnhancedInputComponent->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &ASheriff::Turn);
	EnhancedInputComponent->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &ASheriff::LookUp);
	EnhancedInputComponent->BindAction(InputActions->InputSwitchToFirst, ETriggerEvent::Triggered, this, &ASheriff::SwitchToFirst);
	EnhancedInputComponent->BindAction(InputActions->InputSwitchToSecond, ETriggerEvent::Triggered, this, &ASheriff::SwitchToSecond);
	EnhancedInputComponent->BindAction(InputActions->InputSwitchToThird, ETriggerEvent::Triggered, this, &ASheriff::SwitchToThird);
	EnhancedInputComponent->BindAction(InputActions->InputSwitchToFourth, ETriggerEvent::Triggered, this, &ASheriff::SwitchToFourth);
}

void ASheriff::MoveForward(const FInputActionValue& Value)
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

void ASheriff::MoveRight(const FInputActionValue& Value)
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

void ASheriff::Turn(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void ASheriff::LookUp(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}

void ASheriff::SwitchToFirst()
{
	if (!HasAuthority())
	{
		ServerSwitchToFirst();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[3];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::SwitchToSecond()
{
	if (!HasAuthority())
	{
		ServerSwitchToSecond();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[0];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::SwitchToThird()
{
	if (!HasAuthority())
	{
		ServerSwitchToThird();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[1];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::SwitchToFourth()
{
	if (!HasAuthority())
	{
		ServerSwitchToFourth();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[2];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::ServerSwitchToFirst_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[3];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::ServerSwitchToSecond_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[0];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::ServerSwitchToThird_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[1];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}

void ASheriff::ServerSwitchToFourth_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (Controller != nullptr)
		{
			ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState)
			{
				ASheriff* Sheriff = TownGameState->GetSheriffList()[2];
				if (Sheriff)
				{
					Controller->Possess(Sheriff);
				}
			}
		}
	}
}
