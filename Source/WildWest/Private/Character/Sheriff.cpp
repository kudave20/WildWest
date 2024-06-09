// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Sheriff.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Character/Gunman.h"
#include "Game/TownGameMode.h"
#include "Game/TownGameState.h"
#include "Game/WildWestGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/TownPlayerController.h"
#include "WildWestTypes/ScreenIndex.h"

ASheriff::ASheriff()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;

	GetMesh()->bOwnerNoSee = true;
}

void ASheriff::BeginPlay()
{
	Super::BeginPlay();
	check(SheriffContext);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SheriffContext, 0);
		}
	}
	
	WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	PreviousDirection = GetActorRotation();
	ControlTime = InitialControlTime;
}

void ASheriff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WildWestGameInstance = WildWestGameInstance == nullptr ? GetGameInstance<UWildWestGameInstance>() : WildWestGameInstance;
	if (WildWestGameInstance && !WildWestGameInstance->bLastChase)
	{
		UpdateControlTime(DeltaTime);
	}

	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr)
	{
		bUpdateViewport = false;
		return;
	}

	if (HasAuthority())
	{
		if (!IsPawnControlled() && LineOfSightTo(TownGameState->Gunman))
		{
			LookAt(TownGameState->Gunman);
			bUpdateViewport = true;
		}
		else
		{
			bUpdateViewport = false;
		}
	}
	
	UpdateViewport();
}

void ASheriff::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(FirstSwitchAction, ETriggerEvent::Started, this, &ASheriff::FirstSwitch);
	EnhancedInputComponent->BindAction(SecondSwitchAction, ETriggerEvent::Started, this, &ASheriff::SecondSwitch);
	EnhancedInputComponent->BindAction(ThirdSwitchAction, ETriggerEvent::Started, this, &ASheriff::ThirdSwitch);
	EnhancedInputComponent->BindAction(FourthSwitchAction, ETriggerEvent::Started, this, &ASheriff::FourthSwitch);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASheriff::Interact);
}

void ASheriff::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASheriff, SheriffIndex);
	DOREPLIFETIME(ASheriff, bUpdateViewport);
}

void ASheriff::Restart()
{
	Super::Restart();

	PossessingController = Cast<ATownPlayerController>(Controller);
	if (PossessingController == nullptr) return;
	
	PossessingController->SetInputEnabled(true);
	
	TownGameState = GetWorld()->GetGameState<ATownGameState>();
	if (!PossessingController->bCompleteSetup && TownGameState)
	{
		PossessingController->SheriffHUDSetup();
		PossessingController->bCompleteSetup = true;
		InitialSwitch(TownGameState->SheriffList.Num());
	}
	else if (PossessingController->bCompleteSetup)
	{
		EScreenIndex ScreenIndex = static_cast<EScreenIndex>(SheriffIndex);
		PossessingController->SetCurrentScreenIndex(ScreenIndex);
		PossessingController->SetSheriffHUDScreen(SheriffIndex + 1);
	}
}

void ASheriff::UnPossessed()
{
	Super::UnPossessed();

	PossessingController = nullptr;
}

void ASheriff::UpdateViewport()
{
	if (!bUpdateViewport && bWasDetectingPreviousFrame)
	{
		if (IsPawnControlled())
		{
			HideSheriffHUDViewport(true);
		}
		else
		{
			HideSheriffHUDViewport(false);
		}
	}
	else if (bUpdateViewport)
	{
		UpdateSheriffHUDViewport();
	}
}

void ASheriff::UpdateSheriffHUDViewport()
{
	FirstPlayerController = FirstPlayerController == nullptr ? Cast<ATownPlayerController>(GetWorld()->GetFirstPlayerController()) : FirstPlayerController;
	if (FirstPlayerController == nullptr) return;

	ASheriff* Detector = FirstPlayerController->GetDetector();
	if (Detector == nullptr || Detector == this)
	{
		FirstPlayerController->SetSheriffHUDViewport(SheriffIndex);
		FirstPlayerController->Detect(this, ViewportThreshold);
		bWasDetectingPreviousFrame = true;
	}
}

void ASheriff::HideSheriffHUDViewport()
{
	FirstPlayerController->HideSheriffHUDViewport();
	FirstPlayerController->Detect(nullptr, 0);
	bWasDetectingPreviousFrame = false;
}

void ASheriff::HideSheriffHUDViewport(bool bHideImmediately)
{
	FirstPlayerController = FirstPlayerController == nullptr ? Cast<ATownPlayerController>(GetWorld()->GetFirstPlayerController()) : FirstPlayerController;
	if (FirstPlayerController == nullptr) return;

	if (bHideImmediately)
	{
		HideSheriffHUDViewport();
	}
	else if (!FirstPlayerController->IsDetecting())
	{
		HideSheriffHUDViewport();
	}
}

void ASheriff::LookAt(AActor* Other)
{
	if (Other == nullptr) return;

	FRotator Rotation = (Other->GetActorLocation() - GetActorLocation()).Rotation();
	SetActorRotation(Rotation);
}

void ASheriff::InitialSwitch(int32 CurrentSheriffNum)
{
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr) return;

	int32 RandomIndex = FMath::RandRange(1, CurrentSheriffNum);
	ASheriff* NextSheriff = nullptr;

	switch (RandomIndex)
	{
	case 1:
		FirstSwitch();
		NextSheriff = TownGameState->SheriffList[0];
		break;
	case 2:
		SecondSwitch();
		NextSheriff = TownGameState->SheriffList[1];
		break;
	case 3:
		ThirdSwitch();
		NextSheriff = TownGameState->SheriffList[2];
		break;
	case 4:
		FourthSwitch();
		NextSheriff = TownGameState->SheriffList[3];
		break;
	}

	if (CurrentSheriffNum == 1 && NextSheriff)
	{
		Stun();
	}
}

void ASheriff::Stun()
{
	PossessingController = PossessingController == nullptr ? Cast<ATownPlayerController>(Controller) : PossessingController;
	if (PossessingController)
	{
		PossessingController->StartStun();
	}
}

void ASheriff::FirstSwitch()
{
	if (CanSwitch(EScreenIndex::ECI_First))
	{
		ServerSwitch(EScreenIndex::ECI_First);
	}
}

void ASheriff::SecondSwitch()
{
	if (CanSwitch(EScreenIndex::ECI_Second))
	{
		ServerSwitch(EScreenIndex::ECI_Second);
	}
}

void ASheriff::ThirdSwitch()
{
	if (CanSwitch(EScreenIndex::ECI_Third))
	{
		ServerSwitch(EScreenIndex::ECI_Third);
	}
}

void ASheriff::FourthSwitch()
{
	if (CanSwitch(EScreenIndex::ECI_Fourth))
	{
		ServerSwitch(EScreenIndex::ECI_Fourth);
	}
}

void ASheriff::ServerSwitch_Implementation(EScreenIndex ScreenIndex)
{
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr) return;

	ASheriff* Sheriff = TownGameState->SheriffList[static_cast<uint8>(ScreenIndex)];
	if (Sheriff)
	{
		GetCharacterMovement()->StopMovementImmediately();
		PreviousDirection = Controller->GetControlRotation();
		Controller->Possess(Sheriff);
		Sheriff->Controller->SetControlRotation(Sheriff->GetPreviousDirection());
		Sheriff->Controller->ClientSetRotation(Sheriff->GetPreviousDirection());
	}
}

bool ASheriff::CanSwitch(EScreenIndex ScreenIndex)
{
	uint8 Index = (uint8)ScreenIndex + 1;
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	
	if (TownGameState == nullptr) return false;
	if (Index > TownGameState->SheriffList.Num() || TownGameState->SheriffList[Index - 1] == nullptr) return false;

	PossessingController = PossessingController == nullptr ? Cast<ATownPlayerController>(Controller) : PossessingController;
	if (PossessingController == nullptr || PossessingController->GetCurrentScreenIndex() == ScreenIndex) return false;

	return true;
}

void ASheriff::Interact()
{
	if (!bInputEnabled || !TraceGunman()) return;
	
	ServerInteract();
}

void ASheriff::ServerInteract_Implementation()
{
	EnterDuel();
}

bool ASheriff::TraceGunman()
{
	FHitResult TraceResult;
	FVector Start = Camera->GetComponentLocation();
	FVector Direction = Camera->GetComponentRotation().Vector();
	FVector End = Start + Direction * ArmLength;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		TraceResult,
		Start,
		End,
		ECC_Visibility,
		TraceParams
		);

	if (TraceResult.GetActor() == nullptr || !TraceResult.GetActor()->IsA(AGunman::StaticClass())) return false;
	else return true;
}

bool ASheriff::LineOfSightTo(AActor* Other) const
{
	if (!Other)
	{
		return false;
	}
	
	FVector ViewPoint;
	FRotator ViewRotation;
	GetActorEyesViewPoint(ViewPoint, ViewRotation);
	
	float distSq = (Other->GetActorLocation() - ViewPoint).SizeSquared();
	if (distSq > SightLength * SightLength)
	{
		return false;
	}

	FCollisionQueryParams CollisionParms(SCENE_QUERY_STAT(LineOfSight), true, Other);
	CollisionParms.AddIgnoredActor(this);
	FVector TargetLocation = Other->GetTargetLocation();
	bool bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, TargetLocation, ECC_Visibility, CollisionParms);
	if (!bHit)
	{
		return true;
	}

	float OtherRadius, OtherHeight;
	Other->GetSimpleCollisionCylinder(OtherRadius, OtherHeight);
	
	bHit = GetWorld()->LineTraceTestByChannel(ViewPoint, Other->GetActorLocation() + FVector(0.f,0.f,OtherHeight), ECC_Visibility, CollisionParms);
	return !bHit;
}

void ASheriff::EnterDuel()
{
	WildWestGameInstance = WildWestGameInstance == nullptr ? GetGameInstance<UWildWestGameInstance>() : WildWestGameInstance;
	if (WildWestGameInstance == nullptr) return;
	
	TownGameState = TownGameState == nullptr ? GetWorld()->GetGameState<ATownGameState>() : TownGameState;
	if (TownGameState == nullptr || TownGameState->Gunman == nullptr) return;

	WildWestGameInstance->LastTransformList.Empty();
	WildWestGameInstance->LastGunmanTransform = TownGameState->Gunman->GetActorTransform();
	WildWestGameInstance->SheriffDeathList.Empty();

	for (ASheriff* Sheriff : TownGameState->SheriffList)
	{
		if (Sheriff)
		{
			WildWestGameInstance->LastTransformList.Add(Sheriff->GetActorTransform());
			if (Sheriff != this)
			{
				WildWestGameInstance->SheriffDeathList.Add(false);
			}
			else
			{
				WildWestGameInstance->SheriffDeathList.Add(true);
			}
		}
		else
		{
			WildWestGameInstance->LastTransformList.Add(FTransform::Identity);
			WildWestGameInstance->SheriffDeathList.Add(true);
		}
	}

	ATownGameMode* TownGameMode = GetWorld()->GetAuthGameMode<ATownGameMode>();
	if (TownGameMode)
	{
		TownGameMode->TravelToDuel();
	}
}

void ASheriff::UpdateControlTime(float DeltaTime)
{
	if (IsLocallyControlled() && InputEnabled())
	{
		PossessingController = PossessingController == nullptr ? Cast<ATownPlayerController>(Controller) : PossessingController;
		if (PossessingController == nullptr) return;
		
		if (ControlTime > 0.f)
		{
			ControlTime = FMath::Clamp(ControlTime - DeltaTime, 0.f, InitialControlTime);
		}
		else
		{
			PossessingController->SetInputEnabled(false);
		}
		
		PossessingController->SetSheriffHUDGauge(ControlTime / InitialControlTime);
	}
	else
	{
		if (ControlTime < InitialControlTime)
		{
			ControlTime = FMath::Clamp(ControlTime + DeltaTime, 0.f, InitialControlTime);
		}
	}
}
