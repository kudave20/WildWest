// Fill out your copyright notice in the Description page of Project Settings.


#include "Gunman.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "WildWest/Props/Vault.h"
#include "WildWest/HUD/VaultGauge.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Sheriff.h"
#include "Components/Image.h"
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

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(GetMesh());
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGunman::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AGunman::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AGunman::OnSphereEndOverlap);
	}
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

	if (OverlappingVault && !OverlappingVault->GetbIsOpened())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FHitResult HitResult;
			FVector CameraLocation = Camera->GetComponentLocation();
			bool bIsLookingVault = World->LineTraceSingleByChannel(HitResult,
				CameraLocation,
				CameraLocation + AreaSphere->GetScaledSphereRadius() * Camera->GetForwardVector(),
				ECollisionChannel::ECC_Pawn) && Cast<AVault>(HitResult.GetActor());

			if (bIsLookingVault)
			{
				if (!bIsInteracting)
				{
					if (Press == nullptr)
					{
						Press = CreateWidget(World, PressWidget);
					}
					if (Press && !Press->IsInViewport())
					{
						Press->AddToViewport();
					}
				}
				else
				{
					if (Press && Press->IsInViewport())
					{
						Press->RemoveFromParent();
					}
				}
			}
			else
			{
				if (Press && Press->IsInViewport())
				{
					Press->RemoveFromParent();
				}
			}
		}
	}
	else
	{
		if (Press && Press->IsInViewport())
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

void AGunman::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGunman, OverlappingVault);
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
	if (OverlappingVault && !OverlappingVault->GetbIsOpened())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FHitResult HitResult;
			FVector CameraLocation = Camera->GetComponentLocation();
			bool bIsLookingVault = World->LineTraceSingleByChannel(HitResult,
				CameraLocation,
				CameraLocation + AreaSphere->GetScaledSphereRadius() * Camera->GetForwardVector(),
				ECollisionChannel::ECC_Pawn) && Cast<AVault>(HitResult.GetActor());

			if (!bIsLookingVault)
			{
				return;
			}

			if (VaultGauge == nullptr)
			{
				VaultGauge = CreateWidget<UVaultGauge>(World, VaultGaugeWidget);
				if (VaultGauge)
				{
					VaultGauge->AddToViewport();
				}
			}

			float OpenTimer = OverlappingVault->GetOpenTimer();
			float MaxOpenTimer = OverlappingVault->GetMaxOpenTimer();
			if (OpenTimer >= MaxOpenTimer)
			{
				UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
				if (WildWestGameInstance)
				{
					VaultGauge->RemoveFromParent();

					if (HasAuthority())
					{
						OverlappingVault->OpenDoorDelegate.Broadcast();
						WildWestGameInstance->AddVaultOpened(1);
					}
					else
					{
						ServerOpenVault();
					}

					OverlappingVault->SetbIsOpened(true);

					WildWestGameInstance->ReplaceVaultList(OverlappingVault->GetActorLocation(), true);
				}

				return;
			}

			if (VaultGauge)
			{
				if (!VaultGauge->IsInViewport())
				{
					VaultGauge->AddToViewport();
				}

				OverlappingVault->SetOpenTimer(OpenTimer + World->GetDeltaSeconds());

				UImage* Gauge = VaultGauge->GetGauge();
				if (Gauge)
				{
					UMaterialInstanceDynamic* GaugeMaterialInstance = Gauge->GetDynamicMaterial();
					if (GaugeMaterialInstance)
					{
						GaugeMaterialInstance->SetScalarParameterValue(FName("GaugePercent"), OverlappingVault->GetOpenTimer() / MaxOpenTimer);
					}
				}
			}

			bIsInteracting = true;
		}
	}
}

void AGunman::RemoveVaultGauge()
{
	if (VaultGauge)
	{
		VaultGauge->RemoveFromParent();
	}

	bIsInteracting = false;
}

void AGunman::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASheriff* Sheriff = Cast<ASheriff>(OtherActor);
	if (Sheriff)
	{
		Sheriff->SetOverlappingGunman(this);
	}
}

void AGunman::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASheriff* Sheriff = Cast<ASheriff>(OtherActor);
	if (Sheriff)
	{
		Sheriff->SetOverlappingGunman(nullptr);
	}
}

void AGunman::ServerOpenVault_Implementation()
{
	if (OverlappingVault)
	{
		OverlappingVault->OpenDoorDelegate.Broadcast();
		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			WildWestGameInstance->AddVaultOpened(1);
		}
	}
}
