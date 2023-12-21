// Fill out your copyright notice in the Description page of Project Settings.


#include "Sheriff.h"
#include "Camera/CameraComponent.h"
#include "Gunman.h"
#include "ToolContextInterfaces.h"
#include "WildWest/Controller/TownPlayerController.h"
#include "WildWest/GameMode/TownGameMode.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WildWest/Input/InputConfigData.h"
#include "WildWest/WildWestTypes/ScreenIndex.h"

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
		WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		TownGameState = World->GetGameState<ATownGameState>();
		if (WildWestGameInstance && TownGameState && TownGameState->GetSheriffList().Num() == 1)
		{
			WildWestGameInstance->ChaseLastly(true);
		}
	}

	WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	ControlTimer = InitialControlTimer;
	PreviousDirection = GetActorRotation();
}

void ASheriff::Jump()
{
	if (!bIsInputEnabled) return;

	Super::Jump();
}

void ASheriff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WildWestGameInstance = WildWestGameInstance == nullptr ? GetGameInstance<UWildWestGameInstance>() : WildWestGameInstance;
	if (WildWestGameInstance && WildWestGameInstance->IsLastChase()) return;

	if (IsLocallyControlled() && InputEnabled())
	{
		if (ControlTimer > 0)
		{
			ControlTimer -= DeltaTime;
		}
		else
		{
			bIsInputEnabled = false;
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

	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(GetWorld()->GetFirstPlayerController()) : TownPlayerController;
	if (TownPlayerController == nullptr || TownGameState == nullptr) return;

	TownPlayerController->SetSheriffHUDGauge(ControlTimer / InitialControlTimer);
	
	Gunman = Gunman == nullptr ? TownGameState->GetGunman() : Gunman;
	
	bool bShouldRender = false;
	bShouldRender = LineOfSightTo(Gunman);
	for (ASheriff* Sheriff : TownGameState->GetSheriffList())
	{
		if (Sheriff != this)
		{
			bShouldRender = bShouldRender || LineOfSightTo(Sheriff);
		}
	}
	if (bShouldRender)
	{
		TownPlayerController->SetSheriffViewportRendered(SheriffIndex, true);
	}
	else
	{
		TownPlayerController->SetSheriffViewportRendered(SheriffIndex, false);
	}
}

void ASheriff::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PlayerController =  Cast<APlayerController>(GetController());
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
				EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &ASheriff::Jump);
				EnhancedInputComponent->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &ASheriff::MoveForward);
				EnhancedInputComponent->BindAction(InputActions->InputMoveRight, ETriggerEvent::Triggered, this, &ASheriff::MoveRight);
				EnhancedInputComponent->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &ASheriff::Turn);
				EnhancedInputComponent->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &ASheriff::LookUp);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToFirst, ETriggerEvent::Triggered, this, &ASheriff::SwitchToFirst);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToSecond, ETriggerEvent::Triggered, this, &ASheriff::SwitchToSecond);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToThird, ETriggerEvent::Triggered, this, &ASheriff::SwitchToThird);
				EnhancedInputComponent->BindAction(InputActions->InputSwitchToFourth, ETriggerEvent::Triggered, this, &ASheriff::SwitchToFourth);
				EnhancedInputComponent->BindAction(InputActions->InputInteract, ETriggerEvent::Triggered, this, &ASheriff::Interact);
			}
		}
	}
}

void ASheriff::Restart()
{
	Super::Restart();

	if (IsLocallyControlled())
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		TownPlayerController = Cast<ATownPlayerController>(Controller);
		if (TownPlayerController && !TownPlayerController->IsSetupComplete())
		{
			TownGameState = World->GetGameState<ATownGameState>();
			if (TownGameState == nullptr) return;

			TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();
			TownPlayerController->SheriffHUDSetup(this);
			TownPlayerController->SetupComplete(true);
			SetInitialCharacter(SheriffList.Num(), false);
		}
	}
}

void ASheriff::UnPossessed()
{
	Super::UnPossessed();

	TownPlayerController = nullptr;
}

void ASheriff::SetInitialCharacter(int32 CurrentSheriffNum, bool bShouldStun)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr) return;

	int32 RandomIndex = FMath::RandRange(1, CurrentSheriffNum);
	ASheriff* NextSheriff = nullptr;

	switch (RandomIndex)
	{
	case 1:
		SwitchToFirst();
		NextSheriff = TownGameState->GetSheriffList()[0];
		break;
	case 2:
		SwitchToSecond();
		NextSheriff = TownGameState->GetSheriffList()[1];
		break;
	case 3:
		SwitchToThird();
		NextSheriff = TownGameState->GetSheriffList()[2];
		break;
	case 4:
		SwitchToFourth();
		NextSheriff = TownGameState->GetSheriffList()[3];
		break;
	}

	if (bShouldStun && NextSheriff)
	{
		if (NextSheriff->IsLocallyControlled())
		{
			NextSheriff->StartStunTimer();
		}
		else
		{
			NextSheriff->ClientStartStunTimer();
		}
	}
}

void ASheriff::StartStunTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
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
	if (!bIsInputEnabled) return;

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
	if (!bIsInputEnabled) return;

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
	if (!bIsInputEnabled) return;

	AddControllerYawInput(Value.Get<float>());
}

void ASheriff::LookUp(const FInputActionValue& Value)
{
	if (!bIsInputEnabled) return;

	AddControllerPitchInput(Value.Get<float>());
}

void ASheriff::SwitchToFirst()
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController == nullptr) return;

	if (!CanSwitch(1, EScreenIndex::ECI_First)) return;

	ChangeHUDProperly(1, EScreenIndex::ECI_First, TownPlayerController->GetCurrentScreenIndex());

	if (!HasAuthority())
	{
		ServerSwitchToFirst();
		return;
	}

	SwitchCharacter(1, EScreenIndex::ECI_First);
}

void ASheriff::SwitchToSecond()
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController == nullptr) return;

	if (!CanSwitch(2, EScreenIndex::ECI_Second)) return;

	ChangeHUDProperly(2, EScreenIndex::ECI_Second, TownPlayerController->GetCurrentScreenIndex());

	if (!HasAuthority())
	{
		ServerSwitchToSecond();
		return;
	}

	SwitchCharacter(2, EScreenIndex::ECI_Second);
}

void ASheriff::SwitchToThird()
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController == nullptr) return;

	if (!CanSwitch(3, EScreenIndex::ECI_Third)) return;

	ChangeHUDProperly(3, EScreenIndex::ECI_Third, TownPlayerController->GetCurrentScreenIndex());

	if (!HasAuthority())
	{
		ServerSwitchToThird();
		return;
	}

	SwitchCharacter(3, EScreenIndex::ECI_Third);
}

void ASheriff::SwitchToFourth()
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController == nullptr) return;

	if (!CanSwitch(4, EScreenIndex::ECI_Fourth)) return;

	ChangeHUDProperly(4, EScreenIndex::ECI_Fourth, TownPlayerController->GetCurrentScreenIndex());

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

bool ASheriff::CanSwitch(int32 Index, EScreenIndex ScreenIndex)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return false;

	TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState->GetSheriffList().Num() < Index || TownGameState->GetSheriffList()[Index - 1] == nullptr) return false;

	EScreenIndex CurrentScreenIndex = TownPlayerController->GetCurrentScreenIndex();
	if (CurrentScreenIndex == ScreenIndex) return false;

	return true;
}

void ASheriff::SwitchCharacter(int32 Index, EScreenIndex ScreenIndex)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownGameState == nullptr || TownPlayerController == nullptr) return;

	ASheriff* Sheriff = TownGameState->GetSheriffList()[Index - 1];
	if (Sheriff)
	{
		Sheriff->GetCharacterMovement()->StopMovementImmediately();
		PreviousDirection = Controller->GetControlRotation();
		Controller->Possess(Sheriff);
		Sheriff->Controller->SetControlRotation(Sheriff->GetPreviousDirection());
		Sheriff->Controller->ClientSetRotation(Sheriff->GetPreviousDirection());
	}
}

void ASheriff::ChangeHUDProperly(int32 Index, EScreenIndex ScreenIndex, EScreenIndex PreviousScreenIndex)
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController)
	{
		TownPlayerController->SetSheriffHUDScreen(Index);
		TownPlayerController->SetSheriffHUDViewport(ScreenIndex, PreviousScreenIndex);
		TownPlayerController->DrawSheriffViewport(PreviousScreenIndex);
		TownPlayerController->SetCurrentScreenIndex(ScreenIndex);
	}
}

void ASheriff::Interact()
{
	if (!bIsInputEnabled || !TraceTowardGunman()) return;

	if (!HasAuthority())
	{
		ServerInteract();
		return;
	}

	EnterDuel();
}

void ASheriff::ServerInteract_Implementation()
{
	EnterDuel();
}

bool ASheriff::TraceTowardGunman()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return false;

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if (!bScreenToWorld) return false;

	FVector Start = CrosshairWorldPosition;
	float DistanceToCharacter = (GetActorLocation() - Start).Size();
	Start += CrosshairWorldDirection * (DistanceToCharacter + 10.f);

	FVector End = Start + CrosshairWorldDirection * ARM_LENGTH;

	FHitResult HitResult;
	World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility
	);

	if (HitResult.GetActor() == nullptr || !HitResult.GetActor()->IsA(AGunman::StaticClass())) return false;

	return true;
}

bool ASheriff::LineOfSightTo(AActor* OtherActor)
{
	if (OtherActor == nullptr) return false;

	FVector ViewPoint;
	FRotator ViewRotation;
	GetActorEyesViewPoint(ViewPoint, ViewRotation);

	FCollisionQueryParams CollisionParms(SCENE_QUERY_STAT(LineOfSight), true, OtherActor);
	CollisionParms.AddIgnoredActor(this);
	FVector TargetLocation = OtherActor->GetTargetLocation(this);
	bool bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, TargetLocation, ECC_Visibility, CollisionParms);
	if(!bHit)
	{
		return true;
	}
	
	if (!Cast<const APawn>(OtherActor) && Cast<UCapsuleComponent>(OtherActor->GetRootComponent()) == NULL)
	{
		return false;
	}
	float distSq = (OtherActor->GetActorLocation() - ViewPoint).SizeSquared();
	if ( distSq > FARSIGHTTHRESHOLDSQUARED )
	{
		return false;
	}
	if ( !Cast<const APawn>(OtherActor) && (distSq > NEARSIGHTTHRESHOLDSQUARED) ) 
	{
		return false;
	}

	float OtherRadius, OtherHeight;
	OtherActor->GetSimpleCollisionCylinder(OtherRadius, OtherHeight);
	
	bHit = GetWorld()->LineTraceTestByChannel(ViewPoint,  OtherActor->GetActorLocation() + FVector(0.f,0.f,OtherHeight), ECC_Visibility, CollisionParms);
	return !bHit;
}

void ASheriff::EnterDuel()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	TownGameState = TownGameState == nullptr ? World->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr) return;

	Gunman = Gunman == nullptr ? TownGameState->GetGunman() : Gunman;
	TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();

	WildWestGameInstance = WildWestGameInstance == nullptr ? GetGameInstance<UWildWestGameInstance>() : WildWestGameInstance;
	if (WildWestGameInstance == nullptr || Gunman == nullptr) return;

	WildWestGameInstance->GetLastTransformList().Empty();
	WildWestGameInstance->SetLastGunmanTransform(Gunman->GetActorTransform());

	for (ASheriff* Sheriff : SheriffList)
	{
		if (Sheriff && !Sheriff->IsLocallyControlled())
		{
			WildWestGameInstance->GetLastTransformList().Add(Sheriff->GetActorTransform());
		}
		else
		{
			WildWestGameInstance->GetLastTransformList().Add(FTransform::Identity);
		}
	}

	ATownGameMode* TownGameMode = World->GetAuthGameMode<ATownGameMode>();
	if (TownGameMode)
	{
		TownGameMode->TravelToDuel();
	}
}
