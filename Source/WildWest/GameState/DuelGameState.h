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

protected:
	virtual void BeginPlay() override;
	
private:
	EDuelState GunmanDuelState{ EDuelState::EDS_Initial };

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	EDuelState SheriffDuelState{ EDuelState::EDS_Initial };

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class ADuelGunman* DuelGunman;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class ADuelSheriff* DuelSheriff;

	UPROPERTY(EditAnywhere, Category = "Timer")
	int32 InitialDuelTimer;

	UPROPERTY(ReplicatedUsing = OnRep_DuelTimer)
	int32 DuelTimer;

	UFUNCTION()
	void OnRep_DuelTimer();

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Count")
	int32 BulletCount;

	FTimerHandle CurrentDuelTimer;

	UFUNCTION()
	void CurrentDuelTimerFinished();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Duel", meta = (AllowPrivateAccess = "true"))
	bool bIsDuelOver;

	bool bLastDuel;

	UPROPERTY()
	class ADuelPlayerController* DuelPlayerController;

	UPROPERTY()
	ADuelPlayerController* ServerPlayerController;

	UPROPERTY()
	ADuelPlayerController* ClientPlayerController;

	void SetHUDTimer(int32 Timer);

	void ControlHitMontages(APlayerController* Controller);
	void ControlDodgeMontages(APlayerController* Controller);

	UFUNCTION()
	void ControlTimerFinished(APlayerController* Controller);

public:
	FORCEINLINE EDuelState GetGunmanDuelState() const { return GunmanDuelState; }
	FORCEINLINE EDuelState GetSheriffDuelState() const { return SheriffDuelState; }
	FORCEINLINE void SetGunmanDuelState(EDuelState NewDuelState) { GunmanDuelState = NewDuelState; }
	FORCEINLINE void SetSheriffDuelState(EDuelState NewDuelState) { SheriffDuelState = NewDuelState; }
	FORCEINLINE int32 GetDuelTimer() const { return DuelTimer; }
	FORCEINLINE int32 GetBulletCount() const { return BulletCount; }
	FORCEINLINE void SetDuelOver(bool bIsOver) { bIsDuelOver = bIsOver; }
	FORCEINLINE bool IsLastDuel() const { return bLastDuel; }
};
