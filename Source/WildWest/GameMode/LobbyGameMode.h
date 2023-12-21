// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	void TravelToTown();

};
