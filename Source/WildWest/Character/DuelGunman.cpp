// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGunman.h"
#include "Camera/CameraComponent.h"
#include "WildWest/GameState/DuelGameState.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DuelGunmanAnimInstance.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

ADuelGunman::ADuelGunman()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADuelGunman::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		Revolver = World->SpawnActor<AActor>(RevolverClass);

		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("LeftHandSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(Revolver, GetMesh());
		}
	}
}

void ADuelGunman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADuelGunman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADuelGunman::ShootLeft()
{
	if (!HasAuthority())
	{
		ServerShootLeft();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetGunmanDuelState(EDuelState::EDS_Left);
			if (DuelGameState->GetSheriffDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelGunman::ShootMiddle()
{
	if (!HasAuthority())
	{
		ServerShootMiddle();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetGunmanDuelState(EDuelState::EDS_Middle);
			if (DuelGameState->GetSheriffDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelGunman::ShootRight()
{
	if (!HasAuthority())
	{
		ServerShootRight();
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetGunmanDuelState(EDuelState::EDS_Right);
			if (DuelGameState->GetSheriffDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelGunman::MulticastPlayShootMontage_Implementation(float PlayRate)
{
	UDuelGunmanAnimInstance* AnimInstance = Cast<UDuelGunmanAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayShootMontage(PlayRate);
	}
}

void ADuelGunman::ServerShootLeft_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetGunmanDuelState(EDuelState::EDS_Left);
			if (DuelGameState->GetSheriffDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelGunman::ServerShootMiddle_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetGunmanDuelState(EDuelState::EDS_Middle);
			if (DuelGameState->GetSheriffDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}

void ADuelGunman::ServerShootRight_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			DuelGameState->SetGunmanDuelState(EDuelState::EDS_Right);
			if (DuelGameState->GetSheriffDuelState() != EDuelState::EDS_Initial)
			{
				DuelGameState->StartDuel();
			}
		}
	}
}
