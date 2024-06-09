// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DuelPlayerController.h"
#include "LevelSequencePlayer.h"
#include "Camera/CameraActor.h"
#include "Character/DuelSheriff.h"
#include "Game/WildWestGameInstance.h"
#include "Game/DuelGameState.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/DuelGunmanHUD.h"
#include "HUD/DuelGunmanOverlay.h"
#include "HUD/DuelSheriffHUD.h"
#include "HUD/DuelSheriffOverlay.h"
#include "Components/TextBlock.h"
#include "Game/DuelGameMode.h"
#include "HUD/ReturnToMainMenu.h"

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

void ADuelPlayerController::BullsEye()
{
	ALevelSequenceActor* SequenceActor;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, BullsEyeSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	if (SequencePlayer)
	{
		SequencePlayer->Play();
	}
}

void ADuelPlayerController::BullsEyeLevelSequenceFinished()
{
	AActor* Camera = UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass());
	if (Camera)
	{
		SetViewTargetWithBlend(Camera);
		SetDuelOver();
		
		FTimerHandle FinishTimer;
		GetWorldTimerManager().SetTimer(
			FinishTimer,
			this,
			&ADuelPlayerController::GameFinished,
			FinishTime
			);
	}
}

void ADuelPlayerController::GameFinished()
{
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState == nullptr) return;

	if (DuelGameState->IsDuelOver())
	{
		UUserWidget* GunmanVictory = CreateWidget(this, GunmanVictoryClass);
		if (GunmanVictory)
		{
			GunmanVictory->AddToViewport();
		}
		UReturnToMainMenu* ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuClass);
		if (ReturnToMainMenu)
		{
			ReturnToMainMenu->MenuSetup();
		}
	}
	else if (HasAuthority())
	{
		ADuelGameMode* DuelGameMode = GetWorld()->GetAuthGameMode<ADuelGameMode>();
		if (DuelGameMode)
		{
			DuelGameMode->TravelToTown();
		}
	}
}

void ADuelPlayerController::ClientBullsEye_Implementation()
{
	BullsEye();
}

void ADuelPlayerController::Dodge()
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), Cameras);
	SetViewTargetWithBlend(Cameras[1]);
	
	FTimerHandle DodgeTimer;
	GetWorldTimerManager().SetTimer(
		DodgeTimer,
		this,
		&ADuelPlayerController::DodgeFinished,
		DodgeTime
		);
}

void ADuelPlayerController::DodgeFinished()
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), Cameras);
	SetViewTargetWithBlend(Cameras[0]);

	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState && !DuelGameState->IsDuelOver())
	{
		DuelGunmanHUD = Cast<ADuelGunmanHUD>(GetHUD());
		if (DuelGunmanHUD)
		{
			DuelGunmanHUD->AddDuelGunmanOverlay();
			StartNextDuel(DuelGameState);
		}
		DuelSheriffHUD = Cast<ADuelSheriffHUD>(GetHUD());
		if (DuelSheriffHUD)
		{
			DuelSheriffHUD->AddDuelSheriffOverlay();
			StartNextDuel(DuelGameState);
		}
	}
}

void ADuelPlayerController::StartNextDuel(ADuelGameState* DuelGameState)
{
	if (HasAuthority())
	{
		DuelGameState->ResetDuelState();
		DuelGameState->StartDuelTimer();
	}
}

void ADuelPlayerController::ClientDodge_Implementation()
{
	Dodge();
}

void ADuelPlayerController::Fire()
{
	FTimerHandle FireReadyTimer;
	GetWorldTimerManager().SetTimer(
		FireReadyTimer,
		this,
		&ADuelPlayerController::FireReadyFinished,
		FireReadyTime
		);
}

void ADuelPlayerController::FireReadyFinished()
{
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState)
	{
		FTransform SpawnTransform;
		switch (DuelGameState->GetSheriffDuelState())
		{
		case EDuelState::EDS_Left:
			SpawnTransform = LeftSpawnTransform;
			break;
		case EDuelState::EDS_Middle:
			SpawnTransform = MiddleSpawnTransform;
			break;
		case EDuelState::EDS_Right:
			SpawnTransform = RightSpawnTransform;
			break;
		}

		FireBullet = GetWorld()->SpawnActor<AActor>(BulletClass, SpawnTransform);
		FTimerHandle FireTimer;
		GetWorldTimerManager().SetTimer(
			FireTimer,
			this,
			&ADuelPlayerController::FireFinished,
			FireTime
		);
	}
}

void ADuelPlayerController::FireFinished()
{
	if (FireBullet)
	{
		FireBullet->Destroy();
	}
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState)
	{
		DuelGameState->ResetDuelState();
		if (DuelGameState->DuelSheriff)
		{
			DuelGameState->DuelSheriff->SetIsDead(true);
		}
	}
}

void ADuelPlayerController::ClientFire_Implementation()
{
	Fire();
}

void ADuelPlayerController::Waste()
{
	FTimerHandle WasteReadyTimer;
	GetWorldTimerManager().SetTimer(
		WasteReadyTimer,
		this,
		&ADuelPlayerController::WasteReadyFinished,
		WasteReadyTime
	);
}

void ADuelPlayerController::WasteReadyFinished()
{
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState && DuelGameState->DuelSheriff)
	{
		DuelGameState->DuelSheriff->SetIsSlow(true);
		DuelGameState->DuelSheriff->SetShoot(true);

		ALevelSequenceActor* SequenceActor;
		ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, FinalSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
		if (SequencePlayer)
		{
			SequencePlayer->Play();
		}
	}
	
	FTimerHandle WasteTimer;
	GetWorldTimerManager().SetTimer(
		WasteTimer,
		this,
		&ADuelPlayerController::WasteFinished,
		WasteTime
	);
}

void ADuelPlayerController::WasteFinished()
{
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState && DuelGameState->DuelSheriff)
	{
		DuelGameState->DuelSheriff->PlayAnimMontage(ShootMontage);
		FightBack();
	}
}

void ADuelPlayerController::FightBack()
{
	FTimerHandle FightBackReadyTimer;
	GetWorldTimerManager().SetTimer(
		FightBackReadyTimer,
		this,
		&ADuelPlayerController::FightBackReadyFinished,
		FireReadyTime
	);
}

void ADuelPlayerController::FightBackReadyFinished()
{
	FightBackBullet = GetWorld()->SpawnActor<AActor>(BulletClass, FightBackSpawnTransform);

	FTimerHandle FightBackTimer;
	GetWorldTimerManager().SetTimer(
		FightBackTimer,
		this,
		&ADuelPlayerController::FightBackFinished,
		FireTime
	);
}

void ADuelPlayerController::FightBackFinished()
{
	if (FightBackBullet)
	{
		FightBackBullet->Destroy();
	}
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (DuelGameState && DuelGameState->DuelSheriff)
	{
		DuelGameState->DuelSheriff->SetIsDead(true);
	}
	FTimerHandle FinalTimer;
	GetWorldTimerManager().SetTimer(
		FinalTimer,
		this,
		&ADuelPlayerController::FightBackFinal,
		FireTime
	);
}

void ADuelPlayerController::FightBackFinal()
{
	AActor* Camera = UGameplayStatics::GetActorOfClass(this, ACameraActor::StaticClass());
	if (Camera)
	{
		SetViewTargetWithBlend(Camera);
	}
	UUserWidget* SheriffVictory = CreateWidget(this, SheriffVictoryClass);
	if (SheriffVictory)
	{
		SheriffVictory->AddToViewport();
	}
	UReturnToMainMenu* ReturnToMainMenu = CreateWidget<UReturnToMainMenu>(this, ReturnToMainMenuClass);
	if (ReturnToMainMenu)
	{
		ReturnToMainMenu->MenuSetup();
	}
}

void ADuelPlayerController::ClientWaste_Implementation()
{
	Waste();
}

void ADuelPlayerController::FireBack()
{
	FTimerHandle FireBackReadyTimer;
	GetWorldTimerManager().SetTimer(
		FireBackReadyTimer,
		this,
		&ADuelPlayerController::FireBackReadyFinished,
		WasteReadyTime
	);
}

void ADuelPlayerController::FireBackReadyFinished()
{
	ALevelSequenceActor* SequenceActor;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, FireBackSequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);
	if (SequencePlayer)
	{
		SequencePlayer->Play();
	}
	WasteFinished();
}

void ADuelPlayerController::ClientFireBack_Implementation()
{
	FireBack();
}

void ADuelPlayerController::DuelSelectComplete()
{
	DuelGunmanHUD = Cast<ADuelGunmanHUD>(GetHUD());
	if (DuelGunmanHUD)
	{
		DuelGunmanHUD->RemoveOverlay();
	}
	DuelSheriffHUD = Cast<ADuelSheriffHUD>(GetHUD());
	if (DuelSheriffHUD)
	{
		DuelSheriffHUD->RemoveOverlay();
	}
}

void ADuelPlayerController::ClientDuelSelectComplete_Implementation()
{
	DuelSelectComplete();
}

void ADuelPlayerController::SetDuelOver()
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	ADuelGameState* DuelGameState = GetWorld()->GetGameState<ADuelGameState>();
	if (WildWestGameInstance && DuelGameState)
	{
		DuelGameState->SetDuelOver(WildWestGameInstance->bLastChase);
	}
}
