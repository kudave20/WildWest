// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/DuelGameState.h"
#include "GameFramework/PlayerController.h"
#include "DuelPlayerController.generated.h"

class ULevelSequence;
class ADuelGunmanHUD;
class ADuelSheriffHUD;

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADuelPlayerController();

	void DuelGunmanHUDSetup();
	void DuelSheriffHUDSetup();

	void SetDuelGunmanHUDTimer(int32 Timer);
	void SetDuelSheriffHUDTimer(int32 Timer);

	void SetDuelGunmanHUDBullet(int32 Bullet);
	void SetDuelSheriffHUDBullet(int32 Bullet);

	void BullsEye();
	UFUNCTION(Client, Reliable)
	void ClientBullsEye();
	
	UFUNCTION(BlueprintCallable)
	void BullsEyeLevelSequenceFinished();
	void GameFinished();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FinishTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "Level Sequence")
	TObjectPtr<ULevelSequence> BullsEyeSequence;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> GunmanVictoryClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> ReturnToMainMenuClass;

	void Dodge();
	void StartNextDuel(ADuelGameState* DuelGameState);
	UFUNCTION(Client, Reliable)
	void ClientDodge();

	void DodgeFinished();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float DodgeTime = 2.5f;

	void Fire();
	UFUNCTION(Client, Reliable)
	void ClientFire();

	UFUNCTION()
	void FireReadyFinished();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FireReadyTime = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FTransform LeftSpawnTransform;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FTransform MiddleSpawnTransform;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FTransform RightSpawnTransform;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<AActor> BulletClass;

	void FireFinished();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FireTime = 3.79f;

	UPROPERTY()
	TObjectPtr<AActor> FireBullet;

	void Waste();
	UFUNCTION(Client, Reliable)
	void ClientWaste();

	void WasteReadyFinished();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float WasteReadyTime = 2.5f;

	void WasteFinished();
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	float WasteTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "Level Sequence")
	TObjectPtr<ULevelSequence> FinalSequence;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> ShootMontage;

	void FightBack();
	void FightBackReadyFinished();

	UPROPERTY(EditAnywhere, Category = "Settings")
	FTransform FightBackSpawnTransform;

	UPROPERTY()
	TObjectPtr<AActor> FightBackBullet;

	void FightBackFinished();
	void FightBackFinal();

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FightBackFinalTime = 3.f;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> SheriffVictoryClass;

	void FireBack();
	UFUNCTION(Client, Reliable)
	void ClientFireBack();

	UPROPERTY(EditAnywhere, Category = "Level Sequence")
	TObjectPtr<ULevelSequence> FireBackSequence;

	void FireBackReadyFinished();

	void DuelSelectComplete();
	UFUNCTION(Client, Reliable)
	void ClientDuelSelectComplete();
	
	void SetDuelOver();

private:
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<AHUD> DuelGunmanHUDClass;

	UPROPERTY()
	TObjectPtr<ADuelGunmanHUD> DuelGunmanHUD;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<AHUD> DuelSheriffHUDClass;

	UPROPERTY()
	TObjectPtr<ADuelSheriffHUD> DuelSheriffHUD;

	void DuelGunmanHUDTimerFinished();
	void DuelSheriffHUDTimerFinished();

};
