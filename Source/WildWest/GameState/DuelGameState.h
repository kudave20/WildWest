// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DuelGameState.generated.h"

UENUM(BlueprintType)
enum class EDuelState : uint8
{
	EDS_Initial UMETA(DisplayName = "Initial"),
	EDS_Left UMETA(DisplayName = "Left"),
	EDS_Middle UMETA(DisplayName = "Middle"),
	EDS_Right UMETA(DisplayName = "Right"),

	EDS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void StartDuel();

	UFUNCTION(BlueprintCallable)
	void StartDuelTimer();

	UFUNCTION(BlueprintCallable)
	void ResetDuelState();
	
private:
	EDuelState GunmanDuelState{ EDuelState::EDS_Initial };

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	EDuelState SheriffDuelState{ EDuelState::EDS_Initial };

	class ADuelGunman* DuelGunman;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class ADuelSheriff* DuelSheriff;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	int32 InitialDuelTimer;

	UPROPERTY(Replicated)
	int32 DuelTimer;

	FTimerHandle TimerHandle;

public:
	FORCEINLINE EDuelState GetGunmanDuelState() { return GunmanDuelState; }
	FORCEINLINE EDuelState GetSheriffDuelState() { return SheriffDuelState; }

	FORCEINLINE void SetGunmanDuelState(EDuelState NewDuelState) { GunmanDuelState = NewDuelState; }
	FORCEINLINE void SetSheriffDuelState(EDuelState NewDuelState) { SheriffDuelState = NewDuelState; }

	FORCEINLINE int32 GetDuelTimer() { return DuelTimer; }
};
