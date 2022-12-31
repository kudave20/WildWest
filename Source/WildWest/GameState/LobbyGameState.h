// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(ReplicatedUsing = OnRep_bIsLobbyFull)
	bool bIsLobbyFull{ false };

	UFUNCTION()
	void OnRep_bIsLobbyFull();

	bool bIsGunmanSelected{ false };
	bool bIsSheriffSelected{ false };

public:
	void SetbIsLobbyFull(bool bIsFull);

	FORCEINLINE bool GetbIsGunmanSelected() { return bIsGunmanSelected; }
	FORCEINLINE bool GetbIsSheriffSelected() { return bIsSheriffSelected; }

	void SetbIsGunmanSelected(bool bIsSelected);
	void SetbIsSheriffSelected(bool bIsSelected);
	
};
