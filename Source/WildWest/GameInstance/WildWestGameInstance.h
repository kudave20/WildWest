// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWestGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UWildWestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetupServer(ECharacterState NewServerState);
	void SetupClient(ECharacterState NewClientState);

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ECharacterState ServerCharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ECharacterState ClientCharacterState;

public:
	FORCEINLINE ECharacterState GetServerCharacterState() { return ServerCharacterState; }
	FORCEINLINE ECharacterState GetClientCharacterState() { return ClientCharacterState; }
};
