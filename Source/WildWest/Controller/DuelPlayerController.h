// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DuelPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitDuelDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDuelSelectCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireCompleteDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFightBackDelegate);

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

	UPROPERTY(BlueprintAssignable)
	FOnHitDuelDelegate HitDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDodgeDelegate DodgeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFireDelegate FireDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDuelSelectCompleteDelegate DuelSelectCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFireCompleteDelegate FireCompleteDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFightBackDelegate FightBackDelegate;

	UFUNCTION(Client, Reliable)
	void ClientHitBroadcast();

	UFUNCTION(Client, Reliable)
	void ClientDodgeBroadcast();
	
	UFUNCTION(Client, Reliable)
	void ClientFireBroadcast();

	UFUNCTION(Client, Reliable)
	void ClientDuelSelectCompleteBroadcast();

	UFUNCTION(Client, Reliable)
	void ClientFireCompleteBroadcast();

	UFUNCTION(Client, Reliable)
	void ClientFightBackBroadcast();

	UFUNCTION(BlueprintCallable)
	void SetDuelOver();

	UFUNCTION(Client, Reliable)
	void ClientRemovePlayer();

private:
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<AHUD> DuelGunmanHUDClass;

	UPROPERTY()
	class ADuelGunmanHUD* DuelGunmanHUD;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<AHUD> DuelSheriffHUDClass;

	UPROPERTY()
	class ADuelSheriffHUD* DuelSheriffHUD;

	UFUNCTION()
	void DuelGunmanHUDTimerFinished();

	UFUNCTION()
	void DuelSheriffHUDTimerFinished();

};
