// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TownGameMode.generated.h"

class AGunman;
class ASheriff;
class AVault;

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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GunmanStartClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> GunmanClass;

	UPROPERTY()
	AGunman* Gunman;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SheriffClass;

	UPROPERTY()
	TArray<ASheriff*> SheriffList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> VaultClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> VaultCaseClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	int32 NumberOfVault;
	
};
