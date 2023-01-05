// Fill out your copyright notice in the Description page of Project Settings.


#include "Sheriff.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"
#include "WildWest/Controller/TownPlayerController.h"

ASheriff::ASheriff()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
	Camera->SetIsReplicated(true);

	Screen = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Screen"));
	Screen->SetupAttachment(Camera);
	Screen->SetIsReplicated(true);
}

void ASheriff::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		FTimerHandle TrembleTimerHandle;
		World->GetTimerManager().SetTimer(TrembleTimerHandle, this, &ASheriff::TrembleCharacter, 0.1f, true);
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
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMapping, 0);

			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
			if (EnhancedInputComponent)
			{
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
		}
	}
}

void ASheriff::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASheriff, ControllerDirection);
}

void ASheriff::SetScreenVisibility(bool bNewVisibility)
{
	Screen->SetVisibility(bNewVisibility);
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
	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			if (!HasAuthority())
			{
				ServerSwitchToFirst();
				return;
			}

			EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_First)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[3]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectFirstScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_First);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_First);
					Sheriff->SetCurrentController(Sheriff->GetController());
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}
				}
			}
		}
	}
}

void ASheriff::SwitchToSecond()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			if (!HasAuthority())
			{
				ServerSwitchToSecond();
				return;
			}

			EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_Second)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[0]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectSecondScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_Second);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_Second);
					Sheriff->SetCurrentController(Sheriff->GetController());
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}
				}
			}
		}
	}
}

void ASheriff::SwitchToThird()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			if (!HasAuthority())
			{
				ServerSwitchToThird();
				return;
			}

			EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_Third)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[1]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectThirdScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_Third);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_Third);
					Sheriff->SetCurrentController(Sheriff->GetController());
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}
				}
			}
		}
	}
}

void ASheriff::SwitchToFourth()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			if (!HasAuthority())
			{
				ServerSwitchToFourth();
				return;
			}

			EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_Fourth)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[2]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectFourthScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_Fourth);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_Fourth);
					Sheriff->SetCurrentController(Sheriff->GetController());
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}
				}
			}
		}
	}
}

void ASheriff::TrembleCharacter()
{
	ATownPlayerController* TownPlayerController = Cast<ATownPlayerController>(Controller);
	if (TownPlayerController == nullptr)
	{
		//AddActorLocalOffset(FVector(0.01f, 0.f, 0.f));
		//AddActorLocalOffset(FVector(-0.01f, 0.f, 0.f));
		//AddActorLocalRotation(FRotator(0.01f, 0.f, 0.f));
		//AddActorLocalRotation(FRotator(-0.01f, 0.f, 0.f));
		FVector Direction(0.01f, 0.f, 0.f);
		if (bToggle)
		{
			AddMovementInput(Direction, -1);
			bToggle = false;
		}
		else
		{
			AddMovementInput(Direction, 1);
			bToggle = true;
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
				EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_First)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[3]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectFirstScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_First);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_First);
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}

					UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"),
						Screen->GetComponentRotation().Roll,
						Screen->GetComponentRotation().Pitch,
						Screen->GetComponentRotation().Yaw);
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
				EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_Second)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[0]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectSecondScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_Second);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_Second);
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}

					UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"),
						Screen->GetComponentRotation().Roll,
						Screen->GetComponentRotation().Pitch,
						Screen->GetComponentRotation().Yaw);
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
				EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_Third)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[1]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectThirdScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_Third);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_Third);
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}

					UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"),
						Screen->GetComponentRotation().Roll,
						Screen->GetComponentRotation().Pitch,
						Screen->GetComponentRotation().Yaw);
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
				EScreenIndex ScreenIndex = TownGameState->GetScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_Fourth)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[2]);
				if (Sheriff)
				{
					Sheriff->SetScreenVisibility(false);
					Screen->SetVisibility(true);
					SelectFourthScreenCompleteDelegate.Broadcast(ScreenIndex, EScreenIndex::ECI_Fourth);
					TownGameState->SetScreenIndex(EScreenIndex::ECI_Fourth);
					ControllerDirection = Controller->GetControlRotation();
					Controller->Possess(Sheriff);

					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}

					UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"),
						Screen->GetComponentRotation().Roll,
						Screen->GetComponentRotation().Pitch,
						Screen->GetComponentRotation().Yaw);
				}
			}
		}
	}
}
