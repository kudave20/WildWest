// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DuelSheriffHUD.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API ADuelSheriffHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddDuelSheriffOverlay();

	UFUNCTION(BlueprintCallable)
	void RemoveOverlay();

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> DuelSheriffOverlayClass;

	UPROPERTY()
	class UDuelSheriffOverlay* DuelSheriffOverlay;
};
