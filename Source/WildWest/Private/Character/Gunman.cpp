// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Gunman.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interactable/Vault.h"
#include "Game/WildWestGameInstance.h"
#include "Character/Sheriff.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Player/TownPlayerController.h"

AGunman::AGunman()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;

	GetMesh()->bOwnerNoSee = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AGunman::BeginPlay()
{
	Super::BeginPlay();
	check(GunmanContext);
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(GunmanContext, 0);
		}
	}
}

void AGunman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		FHitResult TraceResult;
		TraceVault(TraceResult);
		DetectedActor = TraceResult.GetActor();
		
		UpdatePressWidget();
		UpdateVaultGauge();

		PreviousDetectedActor = DetectedActor;
	}
}

void AGunman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AGunman::InteractWithVault);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AGunman::StopOpeningVault);
}

void AGunman::TraceVault(FHitResult& TraceResult)
{
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
}

void AGunman::UpdatePressWidget()
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController)
	{
		TargetVault = PreviousDetectedActor != DetectedActor ? Cast<AVault>(DetectedActor) : TargetVault;
		TownPlayerController->SetGunmanPressWidget(TargetVault, PressClass, bIsInteracting);
	}
}

void AGunman::UpdateVaultGauge()
{
	TownPlayerController = TownPlayerController == nullptr ? Cast<ATownPlayerController>(Controller) : TownPlayerController;
	if (TownPlayerController)
	{
		TownPlayerController->SetGunmanVaultGauge(TargetVault, VaultGaugeClass, bIsInteracting);
	}
}

void AGunman::InteractWithVault()
{
	if (TargetVault == nullptr) return;
	
	bIsInteracting = true;
	
	float OpenTimer = TargetVault->GetOpenTimer();
	float MaxOpenTimer = TargetVault->GetMaxOpenTimer();
	if (OpenTimer >= MaxOpenTimer && !TargetVault->IsOpened())
	{
		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			ServerOpenVault();
			TargetVault->Open(true);
		}
	}
	else if (OpenTimer < MaxOpenTimer)
	{
		TargetVault->SetOpenTimer(OpenTimer + GetWorld()->GetDeltaSeconds());
	}
}

void AGunman::ServerOpenVault_Implementation()
{
	if (TargetVault)
	{
		TargetVault->OpenDoor();
		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			WildWestGameInstance->AddVaultOpened(1);
			WildWestGameInstance->ReplaceVaultList(TargetVault->GetActorLocation(), true);
		}
	}
}

void AGunman::StopOpeningVault()
{
	bIsInteracting = false;
}

