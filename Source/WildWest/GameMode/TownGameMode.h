// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TownGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	void TravelToDuel();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SheriffClass;

	UPROPERTY()
	TArray<class ASheriff*> SheriffList;

};
