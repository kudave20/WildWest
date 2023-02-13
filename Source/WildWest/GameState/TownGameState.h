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
	
private:
	UPROPERTY(BlueprintReadWrite, Category = "Gunman", meta = (AllowPrivateAccess = "true"))
	class AGunman* Gunman;

	UPROPERTY(BlueprintReadOnly, Category = "Sheriff", meta = (AllowPrivateAccess = "true"))
	TArray<class ASheriff*> SheriffList;

public:
	FORCEINLINE AGunman* GetGunman() { return Gunman; }

	FORCEINLINE TArray<ASheriff*>& GetSheriffList() { return SheriffList; }

};
