// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WildWestTypes/CharacterState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<AActor> GunmanClass;
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<AActor> SheriffClass;

	UPROPERTY(ReplicatedUsing = OnRep_bIsLobbyFull)
	bool bIsLobbyFull;

	UPROPERTY(ReplicatedUsing = OnRep_ServerCharacterState)
	ECharacterState ServerCharacterState;
	UPROPERTY(ReplicatedUsing = OnRep_ClientCharacterState)
	ECharacterState ClientCharacterState;

	UFUNCTION()
	void OnRep_bIsLobbyFull();

	UFUNCTION()
	void OnRep_ServerCharacterState();
	UFUNCTION()
	void OnRep_ClientCharacterState();

public:
	void SetLobbyFull(bool bFull);
	void SetServerCharacterState(ECharacterState NewState);
	void SetClientCharacterState(ECharacterState NewState);

};
