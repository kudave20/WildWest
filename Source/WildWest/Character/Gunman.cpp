// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunman.h"
#include "Camera/CameraComponent.h"
#include "WildWest/Props/Vault.h"
#include "WildWest/HUD/VaultGauge.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Sheriff.h"
#include "Components/Image.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
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

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

void AGunman::BeginPlay()
{
	Super::BeginPlay();

}

void AGunman::Jump()
{
	if (bIsInteracting)
	{
		return;
	}

	Super::Jump();
}

void AGunman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

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

	if (!bScreenToWorld) return;

	FVector Start = CrosshairWorldPosition;
	float DistanceToCharacter = (GetActorLocation() - Start).Size();
	Start += CrosshairWorldDirection * (DistanceToCharacter + 10.f);

	FVector End = Start + CrosshairWorldDirection * ARM_LENGTH;

	FHitResult HitResult;
	World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_Visibility
	);

	bIsLookingVault = HitResult.GetActor() && HitResult.GetActor()->IsA(AVault::StaticClass());

	if (!bIsLookingVault)
	{
		if (Press)
		{
			Press->RemoveFromParent();
		}

		return;
	}

	TargetVault = TargetVault == nullptr ? Cast<AVault>(HitResult.GetActor()) : TargetVault;
	if (TargetVault->GetbIsOpened()) return;

	if (!bIsInteracting)
	{
		if (Press == nullptr)
		{
			Press = CreateWidget(World->GetFirstPlayerController(), PressClass);
		}
		if (Press && !Press->IsInViewport())
		{
			Press->AddToViewport();
		}
	}
	else
	{
		if (Press)
		{
			Press->RemoveFromParent();
		}
	}
}

void AGunman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
				EnhancedInputComponent->BindAction(InputActions->InputJump, ETriggerEvent::Triggered, this, &AGunman::Jump);
				EnhancedInputComponent->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &AGunman::MoveForward);
				EnhancedInputComponent->BindAction(InputActions->InputMoveRight, ETriggerEvent::Triggered, this, &AGunman::MoveRight);
				EnhancedInputComponent->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &AGunman::Turn);
				EnhancedInputComponent->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &AGunman::LookUp);
				EnhancedInputComponent->BindAction(InputActions->InputInteract, ETriggerEvent::Triggered, this, &AGunman::OpenVault);
				EnhancedInputComponent->BindAction(InputActions->InputInteract, ETriggerEvent::Completed, this, &AGunman::RemoveVaultGauge);
			}
		}
	}
}

void AGunman::MoveForward(const FInputActionValue& Value)
{
	if (bIsInteracting)
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

void AGunman::MoveRight(const FInputActionValue& Value)
{
	if (bIsInteracting)
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

void AGunman::Turn(const FInputActionValue& Value)
{
	if (bIsInteracting)
	{
		return;
	}

	AddControllerYawInput(Value.Get<float>());
}

void AGunman::LookUp(const FInputActionValue& Value)
{
	if (bIsInteracting)
	{
		return;
	}

	AddControllerPitchInput(Value.Get<float>());
}

void AGunman::OpenVault()
{
	if (!bIsLookingVault) return;

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	if (VaultGauge == nullptr)
	{
		VaultGauge = CreateWidget<UVaultGauge>(World->GetFirstPlayerController(), VaultGaugeClass);
		if (VaultGauge)
		{
			VaultGauge->AddToViewport();
		}
	}

	float OpenTimer = TargetVault->GetOpenTimer();
	float MaxOpenTimer = TargetVault->GetMaxOpenTimer();
	if (OpenTimer >= MaxOpenTimer)
	{
		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance && !TargetVault->GetbIsOpened())
		{
			VaultGauge->RemoveFromParent();

			if (HasAuthority())
			{
				TargetVault->OpenDoor();
				WildWestGameInstance->AddVaultOpened(1);
				WildWestGameInstance->ReplaceVaultList(TargetVault->GetActorLocation(), true);
			}
			else
			{
				ServerOpenVault();
			}

			TargetVault->SetbIsOpened(true);
		}

		TargetVault = nullptr;
		return;
	}

	if (VaultGauge)
	{
		if (!VaultGauge->IsInViewport())
		{
			VaultGauge->AddToViewport();
		}

		TargetVault->SetOpenTimer(OpenTimer + World->GetDeltaSeconds());

		UImage* Gauge = VaultGauge->GetGauge();
		if (Gauge)
		{
			UMaterialInstanceDynamic* GaugeMaterialInstance = Gauge->GetDynamicMaterial();
			if (GaugeMaterialInstance)
			{
				GaugeMaterialInstance->SetScalarParameterValue(FName("GaugePercent"), TargetVault->GetOpenTimer() / MaxOpenTimer);
			}
		}
	}

	bIsInteracting = true;
}

void AGunman::RemoveVaultGauge()
{
	if (VaultGauge)
	{
		VaultGauge->RemoveFromParent();
	}

	bIsInteracting = false;
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
