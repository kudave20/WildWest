// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.h"
#include "TownGameState.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	void SheriffListSetup();
	
private:
	UPROPERTY()
	TArray<class ASheriff*> SheriffList;

public:
	FORCEINLINE TArray<ASheriff*> GetSheriffList() { return SheriffList; }
};
