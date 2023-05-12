// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

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

protected:
	virtual void NativeConstruct() override;

private:
	TArray<AActor*> SheriffList;
};
