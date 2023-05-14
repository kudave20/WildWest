// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriff.h"
#include "Camera/CameraComponent.h"
#include "WildWest/GameState/DuelGameState.h"
#include "DuelSheriffAnimInstance.h"
#include "Engine/SkeletalMeshSocket.h"
#include "WildWest/Controller/DuelPlayerController.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "WildWest/Input/InputConfigData.h"

ADuelSheriff::ADuelSheriff()
{
	PrimaryActorTick.bCanEverTick = true;

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

}

void ADuelSheriff::Restart()
{
	Super::Restart();

	if (IsLocallyControlled())
	{
		ADuelPlayerController* DuelPlayerController = Cast<ADuelPlayerController>(Controller);
		if (DuelPlayerController)
		{
			DuelPlayerController->DuelSheriffHUDSetup();
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

void ADuelSheriff::MulticastPlayDodgeLeftMontage_Implementation(float PlayRate)
{
	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayDodgeLeftMontage(PlayRate);
	}
}

void ADuelSheriff::MulticastPlayDodgeRightMontage_Implementation(float PlayRate)
{
	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayDodgeRightMontage(PlayRate);
	}
}

void ADuelSheriff::MulticastSetShoot_Implementation(bool bShoot)
{
	SpawnRevolver();

	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetbShoot(bShoot);
	}
}

void ADuelSheriff::MulticastSetIsSlow_Implementation(bool bIsSlow)
{
	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetbIsSlow(bIsSlow);
	}
}

void ADuelSheriff::SetIsDead(bool bIsDead)
{
	if (Controller)
	{
		Controller->SetControlRotation(FRotator(0));
	}

	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetbIsDead(bIsDead);
	}
}

void ADuelSheriff::SetShoot(bool bShoot)
{
	SpawnRevolver();

	if (Controller)
	{
		Controller->SetControlRotation(FRotator(0, 90, 0));
	}

	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetbShoot(bShoot);
	}
}

void ADuelSheriff::SetIsSlow(bool bIsSlow)
{
	UDuelSheriffAnimInstance* AnimInstance = Cast<UDuelSheriffAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->SetbIsSlow(bIsSlow);
	}
}

void ADuelSheriff::SpawnRevolver()
{
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
