// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyFullDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectServerCharacterDelegate, ECharacterState CharacterState);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectClientCharacterDelegate, ECharacterState CharacterState);

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

	FOnSelectServerCharacterDelegate SelectServerCharacterDelegate;
	FOnSelectClientCharacterDelegate SelectClientCharacterDelegate;

	UFUNCTION(Client, Reliable)
	void ClientBroadcastServerCharacter(ECharacterState NewCharacterState);

	UFUNCTION(Server, Reliable)
	void ServerGunmanButtonClicked();

	UFUNCTION(Server, Reliable)
	void ServerSheriffButtonClicked();

};
