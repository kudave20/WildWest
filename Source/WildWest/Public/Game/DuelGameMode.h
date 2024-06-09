// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DuelGameMode.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void TravelToLobby();
	void TravelToTown();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	UPROPERTY()
	AActor* Camera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DuelGunmanClass;

	UPROPERTY()
	class ADuelGunman* DuelGunman;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DuelSheriffClass;

	UPROPERTY()
	class ADuelSheriff* DuelSheriff;

	void PossessDuelGunman(APlayerController* NewPlayer);
	void PossessDuelSheriff(APlayerController* NewPlayer);
};
