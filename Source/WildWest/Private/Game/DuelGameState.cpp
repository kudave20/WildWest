// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DuelGameState.h"
#include "Character/DuelGunman.h"
#include "Character/DuelSheriff.h"
#include "Player/DuelPlayerController.h"
#include "Game/WildWestGameInstance.h"
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

		ServerPlayerController->Fire();
		ClientPlayerController->ClientFire();

		ServerPlayerController->BullsEye();
		ClientPlayerController->ClientBullsEye();
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

		ServerPlayerController->Dodge();
		ClientPlayerController->ClientDodge();

		BulletCount--;
		SetHUDBullet(BulletCount);
		if (BulletCount == 0)
		{
			ServerPlayerController->Waste();
			ClientPlayerController->ClientWaste();

			bIsDuelOver = true;
			GetWorldTimerManager().ClearTimer(CurrentDuelTimer);
		}
	}

	ServerPlayerController->DuelSelectComplete();
	ClientPlayerController->ClientDuelSelectComplete();
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
			ServerPlayerController->FireBack();
			ClientPlayerController->ClientFireBack();

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
	SetHUDBullet(BulletCount);

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

void ADuelGameState::OnRep_BulletCount()
{
	SetHUDBullet(BulletCount);
}

void ADuelGameState::SetHUDTimer(int32 Timer)
{
	UWorld* World = GetWorld();
	if (World)
	{
		DuelPlayerController = DuelPlayerController == nullptr ? World->GetFirstPlayerController<ADuelPlayerController>() : DuelPlayerController;
		if (DuelPlayerController && DuelPlayerController->GetHUD())
		{
			DuelPlayerController->SetDuelGunmanHUDTimer(Timer);
			DuelPlayerController->SetDuelSheriffHUDTimer(Timer);
		}
	}
}

void ADuelGameState::SetHUDBullet(int32 Bullet)
{
	UWorld* World = GetWorld();
	if (World)
	{
		DuelPlayerController = DuelPlayerController == nullptr ? World->GetFirstPlayerController<ADuelPlayerController>() : DuelPlayerController;
		if (DuelPlayerController && DuelPlayerController->GetHUD())
		{
			DuelPlayerController->SetDuelGunmanHUDBullet(Bullet);
			DuelPlayerController->SetDuelSheriffHUDBullet(Bullet);
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
