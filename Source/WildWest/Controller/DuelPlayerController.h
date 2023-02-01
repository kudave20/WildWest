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

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADuelPlayerController();

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

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Cutscene")
	class ULevelSequence* MasterSequence;
	
};
