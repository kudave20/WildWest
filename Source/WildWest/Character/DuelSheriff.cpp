// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriff.h"
#include "Camera/CameraComponent.h"
#include "WildWest/GameState/DuelGameState.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

ADuelSheriff::ADuelSheriff()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
}

void ADuelSheriff::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADuelSheriff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADuelSheriff::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
				EnhancedInputComponent->BindAction(InputActions->InputDuelLeft, ETriggerEvent::Triggered, this, &ADuelSheriff::DodgeLeft);
				EnhancedInputComponent->BindAction(InputActions->InputDuelMiddle, ETriggerEvent::Triggered, this, &ADuelSheriff::Reload);
				EnhancedInputComponent->BindAction(InputActions->InputDuelRight, ETriggerEvent::Triggered, this, &ADuelSheriff::DodgeRight);
			}
		}
	}
}

void ADuelSheriff::DodgeLeft()
{
	if (!HasAuthority())
	{
		ServerDodgeLeft();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetSheriffDuelState(EDuelState::EDS_Left);
			if (DuelGameState->GetGunmanDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelSheriff::Reload()
{
	if (!HasAuthority())
	{
		ServerReload();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetSheriffDuelState(EDuelState::EDS_Middle);
			if (DuelGameState->GetGunmanDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelSheriff::DodgeRight()
{
	if (!HasAuthority())
	{
		ServerDodgeRight();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetSheriffDuelState(EDuelState::EDS_Right);
			if (DuelGameState->GetGunmanDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelSheriff::ServerDodgeLeft_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetSheriffDuelState(EDuelState::EDS_Left);
			if (DuelGameState->GetGunmanDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelSheriff::ServerReload_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetSheriffDuelState(EDuelState::EDS_Middle);
			if (DuelGameState->GetGunmanDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelSheriff::ServerDodgeRight_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetSheriffDuelState(EDuelState::EDS_Right);
			if (DuelGameState->GetGunmanDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}
