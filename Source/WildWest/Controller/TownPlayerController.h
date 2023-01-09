// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TownPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessCompleteDelegate);

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(BlueprintAssignable)
	FOnPossessCompleteDelegate PossessCompleteDelegate;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_bIsPossessed)
	bool bIsPossessed{ false };

	UFUNCTION()
	void OnRep_bIsPossessed();
};
