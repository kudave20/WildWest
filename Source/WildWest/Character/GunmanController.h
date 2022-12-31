// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GunmanController.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API AGunmanController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void CreateCharacterSelect();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> CharacterSelectWidget;

	class UCharacterSelect* CharacterSelect;
};
