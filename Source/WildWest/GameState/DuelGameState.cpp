// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameState.h"
#include "WildWest/Character/DuelGunman.h"
#include "WildWest/Character/DuelSheriff.h"
#include "WildWest/Controller/DuelPlayerController.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"

void ADuelGameState::BeginPlay()
{
	Super::BeginPlay();

	DuelTimer = 0;

	if (HasAuthority())
	{
		FTimerHandle StartTimer;
		GetWorldTimerManager().SetTimer(
			StartTimer,
			this,
			&ADuelGameState::StartDuelTimer,
			1.1f
		);
	}
}

void ADuelGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADuelGameState, SheriffDuelState);
	DOREPLIFETIME(ADuelGameState, DuelGunman);
	DOREPLIFETIME(ADuelGameState, DuelSheriff);
	DOREPLIFETIME(ADuelGameState, DuelTimer);
	DOREPLIFETIME(ADuelGameState, BulletCount);
	DOREPLIFETIME(ADuelGameState, bIsDuelOver);
}

void ADuelGameState::StartDuel()
{
	GetWorldTimerManager().ClearTimer(CurrentDuelTimer);

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance == nullptr) return;

	if (PlayerArray[0]->GetPlayerController() == nullptr || PlayerArray[1]->GetPlayerController() == nullptr) return;

	if (GunmanDuelState == SheriffDuelState)
	{
		switch (WildWestGameInstance->GetServerCharacterState())
		{
		case ECharacterState::ECS_Gunman:
			DuelGunman = Cast<ADuelGunman>(PlayerArray[0]->GetPlayerController()->GetPawn());
			DuelSheriff = Cast<ADuelSheriff>(PlayerArray[1]->GetPlayerController()->GetPawn());
			ControlHitMontages(PlayerArray[1]->GetPlayerController());
			break;
		case ECharacterState::ECS_Sheriff:
			DuelGunman = Cast<ADuelGunman>(PlayerArray[1]->GetPlayerController()->GetPawn());
			DuelSheriff = Cast<ADuelSheriff>(PlayerArray[0]->GetPlayerController()->GetPawn());
			ControlHitMontages(PlayerArray[0]->GetPlayerController());
			break;
		}

		ServerPlayerController = ServerPlayerController == nullptr ? Cast<ADuelPlayerController>(PlayerArray[0]->GetPlayerController()) : ServerPlayerController;
		ClientPlayerController = ClientPlayerController == nullptr ? Cast<ADuelPlayerController>(PlayerArray[1]->GetPlayerController()) : ClientPlayerController;

		if (ServerPlayerController == nullptr || ClientPlayerController == nullptr) return;

		ServerPlayerController->FireDelegate.Broadcast();
		ClientPlayerController->ClientFireBroadcast();

		ServerPlayerController->HitDelegate.Broadcast();
		ClientPlayerController->ClientHitBroadcast();
	}
	else
	{
		switch (WildWestGameInstance->GetServerCharacterState())
		{
		case ECharacterState::ECS_Gunman:
			DuelGunman = Cast<ADuelGunman>(PlayerArray[0]->GetPlayerController()->GetPawn());
			DuelSheriff = Cast<ADuelSheriff>(PlayerArray[1]->GetPlayerController()->GetPawn());
			ControlDodgeMontages(PlayerArray[1]->GetPlayerController());
			break;
		case ECharacterState::ECS_Sheriff:
			DuelGunman = Cast<ADuelGunman>(PlayerArray[1]->GetPlayerController()->GetPawn());
			DuelSheriff = Cast<ADuelSheriff>(PlayerArray[0]->GetPlayerController()->GetPawn());
			ControlDodgeMontages(PlayerArray[0]->GetPlayerController());
			break;
		}

		ServerPlayerController = ServerPlayerController == nullptr ? Cast<ADuelPlayerController>(PlayerArray[0]->GetPlayerController()) : ServerPlayerController;
		ClientPlayerController = ClientPlayerController == nullptr ? Cast<ADuelPlayerController>(PlayerArray[1]->GetPlayerController()) : ClientPlayerController;

		if (ServerPlayerController == nullptr || ClientPlayerController == nullptr) return;

		ServerPlayerController->DodgeDelegate.Broadcast();
		ClientPlayerController->ClientDodgeBroadcast();

		BulletCount--;
		if (BulletCount == 0)
		{
			ServerPlayerController->FireCompleteDelegate.Broadcast();
			ClientPlayerController->ClientFireCompleteBroadcast();

			bIsDuelOver = true;
			GetWorldTimerManager().ClearTimer(CurrentDuelTimer);
		}
	}

	ServerPlayerController->DuelSelectCompleteDelegate.Broadcast();
	ClientPlayerController->ClientDuelSelectCompleteBroadcast();
}

void ADuelGameState::ControlHitMontages(APlayerController* Controller)
{
	if (DuelGunman)
	{
		DuelGunman->MulticastPlayShootMontage(0.25f);

		FTimerHandle ControlTimer;
		FTimerDelegate ControlDelegate;
		switch (SheriffDuelState)
		{
		case EDuelState::EDS_Left:
			GetWorldTimerManager().SetTimer(ControlTimer, FTimerDelegate::CreateLambda([&]()
				{
					if (DuelSheriff)
					{
						DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
					}
				}), 2.0f, false);

			break;
		case EDuelState::EDS_Middle:
			ControlDelegate.BindUFunction(this, FName("ControlTimerFinished"), Controller);
			GetWorldTimerManager().SetTimer(
				ControlTimer,
				ControlDelegate,
				2.0f,
				false
			);

			break;
		case EDuelState::EDS_Right:
			GetWorldTimerManager().SetTimer(ControlTimer, FTimerDelegate::CreateLambda([&]()
				{
					if (DuelSheriff)
					{
						DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
					}
				}), 2.0f, false);

			break;
		}
	}
}

void ADuelGameState::ControlDodgeMontages(APlayerController* Controller)
{
	if (DuelGunman)
	{
		DuelGunman->MulticastPlayShootMontage(1.0f);
	}
	if (DuelSheriff)
	{
		switch (SheriffDuelState)
		{
		case EDuelState::EDS_Left:
			DuelSheriff->MulticastPlayDodgeLeftMontage(1.0f);
			break;
		case EDuelState::EDS_Middle:
			Controller->SetControlRotation(FRotator(0.f, 90.f, 0.f));
			Controller->ClientSetRotation(FRotator(0.f, 90.f, 0.f));
			ServerPlayerController->FightBackDelegate.Broadcast();
			ClientPlayerController->ClientFightBackBroadcast();

			bIsDuelOver = true;
			GetWorldTimerManager().ClearTimer(CurrentDuelTimer);

			DuelSheriff->MulticastSetShoot(true);
			break;
		case EDuelState::EDS_Right:
			DuelSheriff->MulticastPlayDodgeRightMontage(1.0f);
			break;
		}
	}
}

void ADuelGameState::ControlTimerFinished(APlayerController* Controller)
{
	if (DuelSheriff)
	{
		Controller->SetControlRotation(FRotator(0.f, 90.f, 0.f));
		Controller->ClientSetRotation(FRotator(0.f, 90.f, 0.f));

		DuelSheriff->MulticastSetIsSlow(true);
		DuelSheriff->MulticastSetShoot(true);
	}
}

void ADuelGameState::StartDuelTimer()
{
	DuelTimer = InitialDuelTimer;
	SetHUDTimer(DuelTimer);

	GetWorldTimerManager().SetTimer(
		CurrentDuelTimer,
		this,
		&ADuelGameState::CurrentDuelTimerFinished,
		1.0f,
		true
	);
}

void ADuelGameState::CurrentDuelTimerFinished()
{
	DuelTimer--;
	SetHUDTimer(DuelTimer);

	if (DuelTimer == 0)
	{
		if (GunmanDuelState == EDuelState::EDS_Initial)
		{
			GunmanDuelState = EDuelState::EDS_Middle;
		}
		if (SheriffDuelState == EDuelState::EDS_Initial)
		{
			SheriffDuelState = EDuelState::EDS_Middle;
		}

		StartDuel();
	}
}

void ADuelGameState::OnRep_DuelTimer()
{
	SetHUDTimer(DuelTimer);
}

void ADuelGameState::SetHUDTimer(int32 Timer)
{
	UWorld* World = GetWorld();
	if (World)
	{
		DuelPlayerController = DuelPlayerController == nullptr ? World->GetFirstPlayerController<ADuelPlayerController>() : DuelPlayerController;
		if (DuelPlayerController && DuelPlayerController->GetHUD())
		{
			DuelPlayerController->SetDuelGunmanHUDTimer(DuelTimer);
			DuelPlayerController->SetDuelSheriffHUDTimer(DuelTimer);
		}
	}
}

void ADuelGameState::ResetDuelState()
{
	if (HasAuthority())
	{
		GunmanDuelState = EDuelState::EDS_Initial;
		SheriffDuelState = EDuelState::EDS_Initial;
	}
}
