// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DuelPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartDuelDelegate);

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

	UFUNCTION(NetMulticast, Reliable)
	void MulticastBroadcast();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Cutscene")
	class ULevelSequence* MasterSequence;
	
};
