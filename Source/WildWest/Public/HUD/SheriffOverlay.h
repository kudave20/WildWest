// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SheriffOverlay.generated.h"

class UTextBlock;
class UGameViewportWidget;
class UImage;
class UBorder;

/**
 * 
 */
UCLASS()
class WILDWEST_API USheriffOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScreenIndex;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ViewportIndex;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameViewportWidget> Viewport;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Border;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Gauge;

	void OverlaySetup();

};
