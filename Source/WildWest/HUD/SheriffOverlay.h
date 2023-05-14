// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SheriffOverlay.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API USheriffOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ScreenIndex;

	UPROPERTY(meta = (BindWidget))
	class UGameViewportWidget* FirstViewport;

	UPROPERTY(meta = (BindWidget))
	UGameViewportWidget* SecondViewport;

	UPROPERTY(meta = (BindWidget))
	UGameViewportWidget* ThirdViewport;

	UPROPERTY(meta = (BindWidget))
	UGameViewportWidget* FourthViewport;

	UPROPERTY(meta = (BindWidget))
	class UImage* Gauge;

	UPROPERTY(meta = (BindWidget))
	UImage* FirstDeadScreen;

	UPROPERTY(meta = (BindWidget))
	UImage* SecondDeadScreen;

	UPROPERTY(meta = (BindWidget))
	UImage* ThirdDeadScreen;

	UPROPERTY(meta = (BindWidget))
	UImage* FourthDeadScreen;

	void OverlaySetup();

};
