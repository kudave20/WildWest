// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DuelGunman.h"
#include "Game/DuelGameState.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Character/DuelGunmanAnimInstance.h"
#include "Player/DuelPlayerController.h"

ADuelGunman::ADuelGunman()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADuelGunman::BeginPlay()
{
	Super::BeginPlay();

	Revolver = GetWorld()->SpawnActor<AActor>(RevolverClass);

	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("LeftHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(Revolver, GetMesh());
	}
}

void ADuelGunman::Restart()
{
	Super::Restart();
	
	ADuelPlayerController* DuelPlayerController = Cast<ADuelPlayerController>(Controller);
	if (DuelPlayerController)
	{
		DuelPlayerController->DuelGunmanHUDSetup();
	}
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

void ADuelGunman::SetIsDead(bool bIsDead)
{
	if (Controller)
	{
		Controller->SetControlRotation(FRotator(0, 180, 0));
	}

	UDuelGunmanAnimInstance* AnimInstance = Cast<UDuelGunmanAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetbIsDead(bIsDead);
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
