// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SheriffHUD.generated.h"

class USheriffOverlay;

/**
 * 
 */
UCLASS()
class WILDWEST_API ASheriffHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> SheriffOverlayClass;

	UPROPERTY()
	TObjectPtr<USheriffOverlay> SheriffOverlay;

	void AddSheriffOverlay();

};