// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyFullDelegate);

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnLobbyFullDelegate LobbyFullDelegate;

	UFUNCTION(Server, Reliable)
	void ServerGunmanButtonClicked();

	UFUNCTION(Server, Reliable)
	void ServerSheriffButtonClicked();

private:
	bool bIsSelectingCharacter;

public:
	FORCEINLINE bool GetbIsSelectingCharacter() { return bIsSelectingCharacter; }
	FORCEINLINE void SetbIsSelectingCharacter(bool bIsSelecting) { bIsSelectingCharacter = bIsSelecting; }
	
};
