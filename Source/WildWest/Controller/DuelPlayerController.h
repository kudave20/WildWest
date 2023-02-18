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

	UFUNCTION(Client, Reliable)
	void ClientSetInputModeGameOnly();

	UFUNCTION(BlueprintCallable)
	void SetInitialControlRotation(const FRotator& NewRotation);

	UFUNCTION(BlueprintCallable)
	void SetControllerIndex();

	UFUNCTION(Server, Reliable)
	void ServerSetControllerIndex();

protected:
	virtual void BeginPlay() override;

};
