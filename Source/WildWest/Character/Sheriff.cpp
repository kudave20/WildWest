// Fill out your copyright notice in the Description page of Project Settings.


#include "Sheriff.h"
#include "Camera/CameraComponent.h"
#include "Gunman.h"
#include "WildWest/Controller/TownPlayerController.h"
#include "WildWest/GameMode/TownGameMode.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"
#include "Blueprint/UserWidget.h"

ASheriff::ASheriff()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
	Camera->SetIsReplicated(true);
}

void ASheriff::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			if (WildWestGameInstance->GetAliveControllerIndex().Num() == 1)
			{
				bIsAlone = true;
			}
		}
	}

	ControlTimer = InitialControlTimer;
}

void ASheriff::Jump()
{
	if (!bIsInputEnabled)
	{
		return;
	}

	Super::Jump();
}

void ASheriff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAlone)
	{
		return;
	}

	if (IsLocallyControlled())
	{
		if (bIsInputEnabled)
		{
			if (ControlTimer > 0)
			{
				if (InputEnabled())
				{
					ControlTimer -= DeltaTime;
				}
			}
			else
			{
				bIsInputEnabled = false;
			}
		}
	}
	else
	{
		bIsInputEnabled = true;

		if (ControlTimer < InitialControlTimer)
		{
			ControlTimer += DeltaTime;
		}
	}

	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController)
	{
		TownPlayerController->SetSheriffHUDGauge(ControlTimer / InitialControlTimer);
	}
}

void ASheriff::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC =  Cast<APlayerController>(GetController());
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMapping, 0);

			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ASheriff::Jump);
				EnhancedInputComponent->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &ASheriff::MoveForward);
				EnhancedInputComponent->BindAction(InputActions->InputMoveRight, ETriggerEvent::Triggered, this, &ASheriff::MoveRight);
				EnhancedInputComponent->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &ASheriff::Turn);
				EnhancedInputComponent->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &ASheriff::LookUp);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToFirst, ETriggerEvent::Triggered, this, &ASheriff::SwitchToFirst);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToSecond, ETriggerEvent::Triggered, this, &ASheriff::SwitchToSecond);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToThird, ETriggerEvent::Triggered, this, &ASheriff::SwitchToThird);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToFourth, ETriggerEvent::Triggered, this, &ASheriff::SwitchToFourth);
				EnhancedInputComponent->BindAction(InputActions->InputInteract, ETriggerEvent::Triggered, this, &ASheriff::EnterDuel);
			}
		}
	}
}

void ASheriff::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASheriff, CharacterIndex);
	DOREPLIFETIME(ASheriff, bIsAlone);
}

void ASheriff::SelectControlCharacter()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			TArray<int32> ControllableIndexList;
			int ControllableNum = 0;

			for (int32 idx = 0; idx < TownGameState->GetSheriffList().Num(); idx++)
			{
				if (TownGameState->GetSheriffList()[idx])
				{
					ControllableNum++;
					ControllableIndexList.Add(idx);
				}
			}

			int32 RandomIndex = FMath::RandRange(0, ControllableNum - 1);
			ASheriff* NextSheriff = NULL;

			switch (ControllableIndexList[RandomIndex])
			{
			case 0:
				SwitchToFirst();
				NextSheriff = TownGameState->GetSheriffList()[0];
				break;
			case 1:
				SwitchToSecond();
				NextSheriff = TownGameState->GetSheriffList()[1];
				break;
			case 2:
				SwitchToThird();
				NextSheriff = TownGameState->GetSheriffList()[2];
				break;
			case 3:
				SwitchToFourth();
				NextSheriff = TownGameState->GetSheriffList()[3];
				break;
			}

			if (NextSheriff)
			{
				UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
				if (WildWestGameInstance)
				{
					if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Sheriff)
					{
						NextSheriff->StartStunTimer();
					}
					else
					{
						NextSheriff->ClientStartStunTimer();
					}
				}
			}
		}
	}
}

void ASheriff::StartStunTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = PlayerController == nullptr ? Cast<APlayerController>(GetController()) : PlayerController;
		if (PlayerController)
		{
			DisableInput(PlayerController);

			Stun = CreateWidget(World, StunWidget);
			if (Stun)
			{
				Stun->AddToPlayerScreen(1);
			}

			World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					EnableInput(PlayerController);

					if (Stun)
					{
						Stun->RemoveFromParent();
					}
				}), StunTimer, false);
		}
	}
}

void ASheriff::ClientStartStunTimer_Implementation()
{
	StartStunTimer();
}

void ASheriff::MoveForward(const FInputActionValue& Value)
{
	if (!bIsInputEnabled)
	{
		return;
	}

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
	if (!bIsInputEnabled)
	{
		return;
	}

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
	if (!bIsInputEnabled)
	{
		return;
	}

	AddControllerYawInput(Value.Get<float>());
}

void ASheriff::LookUp(const FInputActionValue& Value)
{
	if (!bIsInputEnabled)
	{
		return;
	}

	AddControllerPitchInput(Value.Get<float>());
}

void ASheriff::SwitchToFirst()
{
	ChangeHUDProperly(1, EScreenIndex::ECI_First);

	if (!HasAuthority())
	{
		ServerSwitchToFirst();
		return;
	}

	SwitchCharacter(1, EScreenIndex::ECI_First);
}

void ASheriff::SwitchToSecond()
{
	ChangeHUDProperly(2, EScreenIndex::ECI_Second);

	if (!HasAuthority())
	{
		ServerSwitchToSecond();
		return;
	}

	SwitchCharacter(2, EScreenIndex::ECI_Second);
}

void ASheriff::SwitchToThird()
{
	ChangeHUDProperly(3, EScreenIndex::ECI_Third);

	if (!HasAuthority())
	{
		ServerSwitchToThird();
		return;
	}

	SwitchCharacter(3, EScreenIndex::ECI_Third);
}

void ASheriff::SwitchToFourth()
{
	ChangeHUDProperly(4, EScreenIndex::ECI_Fourth);

	if (!HasAuthority())
	{
		ServerSwitchToFourth();
		return;
	}

	SwitchCharacter(4, EScreenIndex::ECI_Fourth);
}

void ASheriff::ServerSwitchToFirst_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	SwitchCharacter(1, EScreenIndex::ECI_First);
}

void ASheriff::ServerSwitchToSecond_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	SwitchCharacter(2, EScreenIndex::ECI_Second);
}

void ASheriff::ServerSwitchToThird_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	SwitchCharacter(3, EScreenIndex::ECI_Third);
}

void ASheriff::ServerSwitchToFourth_Implementation()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	SwitchCharacter(4, EScreenIndex::ECI_Fourth);
}

void ASheriff::SwitchCharacter(int32 Index, EScreenIndex ScreenIndex)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownGameState == nullptr || TownPlayerController == nullptr) return;

	EScreenIndex CurrentScreenIndex = TownPlayerController->GetCurrentScreenIndex();
	if (CurrentScreenIndex == ScreenIndex)
	{
		return;
	}

	ASheriff* Sheriff = TownGameState->GetSheriffList()[Index - 1];
	if (Sheriff)
	{
		ControllerDirection = Controller->GetControlRotation();
		TownPlayerController->SetCurrentScreenIndex(ScreenIndex);
		Controller->SetControlRotation(ControllerDirection);
		Controller->Possess(Sheriff);

		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			WildWestGameInstance->SetCurrentSheriffIndex(Index);
		}
	}
}

void ASheriff::ChangeHUDProperly(int32 Index, EScreenIndex ScreenIndex)
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController)
	{
		TownPlayerController->SetSheriffHUDScreen(Index);
		TownPlayerController->SetSheriffHUDViewport(ScreenIndex);
	}
}

void ASheriff::EnterDuel()
{
	if (!bIsInputEnabled)
	{
		return;
	}

	if (!HasAuthority())
	{
		ServerEnterDuel();
		return;
	}

	if (OverlappingGunman == nullptr)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			AGunman* Gunman = TownGameState->GetGunman();
			TArray<ASheriff*> SheriffList = TownGameState->GetSheriffList();

			UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
			if (WildWestGameInstance)
			{
				WildWestGameInstance->SetCurrentSheriffIndex(CharacterIndex);
				WildWestGameInstance->GetLastTransformList().Empty();

				for (ASheriff* Sheriff : SheriffList)
				{
					if (Gunman)
					{
						WildWestGameInstance->SetLastGunmanTransform(Gunman->GetActorTransform());
					}

					if (Sheriff)
					{
						WildWestGameInstance->AddLastTransformList(Sheriff->GetActorTransform());
					}
					else
					{
						WildWestGameInstance->AddLastTransformList(FTransform());
					}
				}

				ATownGameMode* TownGameMode = World->GetAuthGameMode<ATownGameMode>();
				if (TownGameMode)
				{
					TownGameMode->TravelToDuel();
				}
			}
		}
	}
}

void ASheriff::ServerEnterDuel_Implementation()
{
	if (OverlappingGunman == nullptr)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
		if (TownGameState)
		{
			AGunman* Gunman = TownGameState->GetGunman();
			TArray<ASheriff*> SheriffList = TownGameState->GetSheriffList();
			
			UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
			if (WildWestGameInstance)
			{
				WildWestGameInstance->SetCurrentSheriffIndex(CharacterIndex);
				WildWestGameInstance->GetLastTransformList().Empty();

				if (Gunman)
				{
					WildWestGameInstance->SetLastGunmanTransform(Gunman->GetActorTransform());
				}

				for (ASheriff* Sheriff : SheriffList)
				{
					if (Sheriff)
					{
						WildWestGameInstance->AddLastTransformList(Sheriff->GetActorTransform());
					}
					else
					{
						WildWestGameInstance->AddLastTransformList(FTransform());
					}
				}

				ATownGameMode* TownGameMode = World->GetAuthGameMode<ATownGameMode>();
				if (TownGameMode)
				{
					TownGameMode->TravelToDuel();
				}
			}
		}
	}
}
