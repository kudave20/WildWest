// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelPlayerController.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/GameState/DuelGameState.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/HUD/DuelGunmanHUD.h"
#include "WildWest/HUD/DuelGunmanOverlay.h"
#include "WildWest/HUD/DuelSheriffHUD.h"
#include "WildWest/HUD/DuelSheriffOverlay.h"
#include "Components/TextBlock.h"

ADuelPlayerController::ADuelPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void ADuelPlayerController::DuelGunmanHUDSetup()
{
	FTimerHandle DuelGunmanHUDTimer;
	GetWorldTimerManager().SetTimer(
		DuelGunmanHUDTimer,
		this,
		&ADuelPlayerController::DuelGunmanHUDTimerFinished,
		1.1f
	);
}

void ADuelPlayerController::DuelSheriffHUDSetup()
{
	FTimerHandle DuelSheriffHUDTimer;
	GetWorldTimerManager().SetTimer(
		DuelSheriffHUDTimer,
		this,
		&ADuelPlayerController::DuelSheriffHUDTimerFinished,
		1.1f
	);
}

void ADuelPlayerController::DuelGunmanHUDTimerFinished()
{
	if (DuelGunmanHUDClass)
	{
		ClientSetHUD(DuelGunmanHUDClass);
		DuelGunmanHUD = Cast<ADuelGunmanHUD>(GetHUD());
		if (DuelGunmanHUD)
		{
			DuelGunmanHUD->AddDuelGunmanOverlay();
		}
	}
}

void ADuelPlayerController::DuelSheriffHUDTimerFinished()
{
	if (DuelSheriffHUDClass)
	{
		ClientSetHUD(DuelSheriffHUDClass);
		DuelSheriffHUD = Cast<ADuelSheriffHUD>(GetHUD());
		if (DuelSheriffHUD)
		{
			DuelSheriffHUD->AddDuelSheriffOverlay();
		}
	}
}

void ADuelPlayerController::SetDuelGunmanHUDTimer(int32 Timer)
{
	DuelGunmanHUD = DuelGunmanHUD == nullptr ? Cast<ADuelGunmanHUD>(GetHUD()) : DuelGunmanHUD;
	bool bHUDValid = DuelGunmanHUD &&
		DuelGunmanHUD->DuelGunmanOverlay &&
		DuelGunmanHUD->DuelGunmanOverlay->TimerText;
	if (bHUDValid)
	{
		FString TimerText = FString::Printf(TEXT("%d"), Timer);
		DuelGunmanHUD->DuelGunmanOverlay->TimerText->SetText(FText::FromString(TimerText));
	}
}

void ADuelPlayerController::SetDuelSheriffHUDTimer(int32 Timer)
{
	DuelSheriffHUD = DuelSheriffHUD == nullptr ? Cast<ADuelSheriffHUD>(GetHUD()) : DuelSheriffHUD;
	bool bHUDValid = DuelSheriffHUD &&
		DuelSheriffHUD->DuelSheriffOverlay &&
		DuelSheriffHUD->DuelSheriffOverlay->TimerText;
	if (bHUDValid)
	{
		FString TimerText = FString::Printf(TEXT("%d"), Timer);
		DuelSheriffHUD->DuelSheriffOverlay->TimerText->SetText(FText::FromString(TimerText));
	}
}

void ADuelPlayerController::SetDuelGunmanHUDBullet(int32 Bullet)
{
	DuelGunmanHUD = DuelGunmanHUD == nullptr ? Cast<ADuelGunmanHUD>(GetHUD()) : DuelGunmanHUD;
	bool bHUDValid = DuelGunmanHUD &&
		DuelGunmanHUD->DuelGunmanOverlay &&
		DuelGunmanHUD->DuelGunmanOverlay->BulletText;
	if (bHUDValid)
	{
		FString BulletText = FString::Printf(TEXT("%d"), Bullet);
		DuelGunmanHUD->DuelGunmanOverlay->BulletText->SetText(FText::FromString(BulletText));
	}
}

void ADuelPlayerController::SetDuelSheriffHUDBullet(int32 Bullet)
{
	DuelSheriffHUD = DuelSheriffHUD == nullptr ? Cast<ADuelSheriffHUD>(GetHUD()) : DuelSheriffHUD;
	bool bHUDValid = DuelSheriffHUD &&
		DuelSheriffHUD->DuelSheriffOverlay &&
		DuelSheriffHUD->DuelSheriffOverlay->BulletText;
	if (bHUDValid)
	{
		FString BulletText = FString::Printf(TEXT("%d"), Bullet);
		DuelSheriffHUD->DuelSheriffOverlay->BulletText->SetText(FText::FromString(BulletText));
	}
}

void ADuelPlayerController::ClientHitBroadcast_Implementation()
{
	HitDelegate.Broadcast();
}

void ADuelPlayerController::ClientDodgeBroadcast_Implementation()
{
	DodgeDelegate.Broadcast();
}

void ADuelPlayerController::ClientFireBroadcast_Implementation()
{
	FireDelegate.Broadcast();
}

void ADuelPlayerController::ClientDuelSelectCompleteBroadcast_Implementation()
{
	DuelSelectCompleteDelegate.Broadcast();
}

void ADuelPlayerController::ClientFireCompleteBroadcast_Implementation()
{
	FireCompleteDelegate.Broadcast();
}

void ADuelPlayerController::ClientFightBackBroadcast_Implementation()
{
	FightBackDelegate.Broadcast();
}

void ADuelPlayerController::SetDuelOver()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
	if (WildWestGameInstance && DuelGameState)
	{
		DuelGameState->SetDuelOver(WildWestGameInstance->IsLastChase());
	}
}

void ADuelPlayerController::ClientRemovePlayer_Implementation()
{
	UGameplayStatics::RemovePlayer(this, true);
}
