// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TownGameState.generated.h"

class ASheriff;
class AGunman;

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	TArray<ASheriff*> SheriffList;
	UPROPERTY(Replicated)
	AGunman* Gunman;

};
