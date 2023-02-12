// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DuelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void TravelToLobby();

	UFUNCTION(BlueprintCallable)
	void TravelToTown();

};
