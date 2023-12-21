// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildWest/WildWestTypes/CharacterState.h"
#include "LobbyPlayerController.generated.h"

class UCharacterSelect;

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void GunmanButtonClicked();
	UFUNCTION(Server, Reliable)
	void ServerGunmanButtonClicked();

	void SheriffButtonClicked();
	UFUNCTION(Server, Reliable)
	void ServerSheriffButtonClicked();

	void AddCharacterSelect();

	void ChangeButtonState(ECharacterState NewState);

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> CharacterSelectClass;

	UPROPERTY()
	UCharacterSelect* CharacterSelect;
};
