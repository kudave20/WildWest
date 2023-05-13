// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TownGameState.generated.h"


/**
 * 
 */
UCLASS()
class WILDWEST_API ATownGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	class AGunman* Gunman;

	UPROPERTY(Replicated)
	TArray<class ASheriff*> SheriffList;

	UPROPERTY()
	APlayerController* PlayerController;

public:
	FORCEINLINE AGunman* GetGunman() const { return Gunman; }
	FORCEINLINE void SetGunman(AGunman* InGunman) { Gunman = InGunman; }
	FORCEINLINE TArray<ASheriff*>& GetSheriffList() { return SheriffList; }

};
