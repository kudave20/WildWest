// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DuelGunmanHUD.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelGunmanHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddDuelGunmanOverlay();

	UFUNCTION(BlueprintCallable)
	void RemoveOverlay();

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> DuelGunmanOverlayClass;

	UPROPERTY()
	class UDuelGunmanOverlay* DuelGunmanOverlay;
};
