// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DuelPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartDuelDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDodgeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireDelegate);

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
	FOnStartDuelDelegate StartDuelDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDodgeDelegate DodgeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFireDelegate FireDelegate;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartDuelBroadcast();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDodgeBroadcast();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFireBroadcast();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Cutscene")
	class ULevelSequence* MasterSequence;
	
};
