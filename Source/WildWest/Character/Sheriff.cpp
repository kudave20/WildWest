// Fill out your copyright notice in the Description page of Project Settings.


#include "Sheriff.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WildWest/Controller/TownPlayerController.h"
#include "WildWest/GameState/TownGameState.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"
#include "Engine/GameEngine.h"

ASheriff::ASheriff()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
	Camera->SetIsReplicated(true);
}

void ASheriff::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		/*SplitscreenData.PlayerData.Add(FPerPlayerSplitscreenData(0.125f, 0.125f, 0.75f, 0.f));
		SplitscreenData.PlayerData.Add(FPerPlayerSplitscreenData(0.125f, 0.125f, 0.875f, 0.f));
		SplitscreenData.PlayerData.Add(FPerPlayerSplitscreenData(0.125f, 0.125f, 0.75f, 0.125f));
		SplitscreenData.PlayerData.Add(FPerPlayerSplitscreenData(0.125f, 0.125f, 0.875f, 0.125f));

		if (GEngine)
		{
			UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
			if (GameEngine)
			{
				//UGameViewportClient* GameViewportClient = World->GetGameViewport();
				UGameViewportClient* GameViewportClient = NewObject<UGameViewportClient>(GEngine, UGameViewportClient::StaticClass());
				if (GameViewportClient)
				{
					UGameInstance* GameInstance = GetGameInstance();
					if (GameInstance)
					{
						GameViewportClient->Init(*GameInstance->GetWorldContext(), GameInstance);
						GameEngine->GameViewport = GameViewportClient;
						GameInstance->GetWorldContext()->GameViewport = GameViewportClient;



						//PlayerList = GetOuterUEngine()->GetGamePlayers(GameViewportClient);

						for (int32 PlayerIdx = 0; PlayerIdx < PlayerList.Num(); PlayerIdx++)
						{
							PlayerList[PlayerIdx]->Size.X = SplitscreenData.PlayerData[PlayerIdx].SizeX;
							PlayerList[PlayerIdx]->Size.Y = SplitscreenData.PlayerData[PlayerIdx].SizeY;
							PlayerList[PlayerIdx]->Origin.X = SplitscreenData.PlayerData[PlayerIdx].OriginX;
							PlayerList[PlayerIdx]->Origin.Y = SplitscreenData.PlayerData[PlayerIdx].OriginY;

							UE_LOG(LogTemp, Warning, TEXT("Test!"));
						}
					}
				}

				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Yellow,
					TEXT("Yes!")
				);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Red,
					TEXT("No!")
				);
			}
		}*/
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

void ASheriff::SetCameraPitchRotation(float PitchValue)
{
	float YawValue = Camera->GetRelativeRotation().Yaw;
	Camera->SetRelativeRotation(FRotator(PitchValue, YawValue, 0.f));
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
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_First)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[0]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_First);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);

					NewController = Sheriff->GetController();
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
	if (!HasAuthority())
	{
		ServerSwitchToSecond();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{

			EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_Second)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[1]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_Second);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);

					NewController = Sheriff->GetController();
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
	if (!HasAuthority())
	{
		ServerSwitchToThird();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_Third)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[2]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}
					
					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_Third);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);

					NewController = Sheriff->GetController();
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
	if (!HasAuthority())
	{
		ServerSwitchToFourth();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

			if (ScreenIndex == EScreenIndex::ECI_Fourth)
			{
				return;
			}

			if (Controller != nullptr)
			{
				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[3]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_Fourth);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);

					NewController = Sheriff->GetController();
					if (NewController)
					{
						NewController->SetControlRotation(Sheriff->GetControllerDirection());
					}
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
				EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_First)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[0]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_First);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);
					Controller->ClientSetRotation(ControllerDirection);

					NewController = Sheriff->GetController();
					if (NewController)
					{
						FRotator NewDirection = Sheriff->GetControllerDirection();
						NewController->SetControlRotation(NewDirection);
						NewController->ClientSetRotation(NewDirection);
					}
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
				EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_Second)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[1]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_Second);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);
					Controller->ClientSetRotation(ControllerDirection);

					NewController = Sheriff->GetController();
					if (NewController)
					{
						FRotator NewDirection = Sheriff->GetControllerDirection();
						NewController->SetControlRotation(NewDirection);
						NewController->ClientSetRotation(NewDirection);
					}
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
				EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_Third)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[2]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_Third);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);
					Controller->ClientSetRotation(ControllerDirection);

					NewController = Sheriff->GetController();
					if (NewController)
					{
						FRotator NewDirection = Sheriff->GetControllerDirection();
						NewController->SetControlRotation(NewDirection);
						NewController->ClientSetRotation(NewDirection);
					}
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
				EScreenIndex ScreenIndex = TownGameState->GetCurrentScreenIndex();

				if (ScreenIndex == EScreenIndex::ECI_Fourth)
				{
					return;
				}

				ASheriff* Sheriff = Cast<ASheriff>(TownGameState->GetSheriffList()[3]);
				if (Sheriff)
				{
					AController* NewController = Sheriff->GetController();
					if (NewController)
					{
						Sheriff->SetCurrentController(NewController);
						Sheriff->SetControllerDirection(NewController->GetControlRotation());
					}

					ControllerDirection = Controller->GetControlRotation();
					TownGameState->SetCurrentScreenIndex(EScreenIndex::ECI_Fourth);
					Controller->Possess(Sheriff);
					Controller = Sheriff->GetCurrentController();
					Controller->Possess(this);
					Sheriff->SetCurrentController(Sheriff->GetController());
					Controller->SetControlRotation(ControllerDirection);
					Controller->ClientSetRotation(ControllerDirection);

					NewController = Sheriff->GetController();
					if (NewController)
					{
						FRotator NewDirection = Sheriff->GetControllerDirection();
						NewController->SetControlRotation(NewDirection);
						NewController->ClientSetRotation(NewDirection);
					}
				}
			}
		}
	}
}
