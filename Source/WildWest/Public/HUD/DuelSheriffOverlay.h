// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DuelSheriffOverlay.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UDuelSheriffOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void OverlaySetup(APlayerController* Controller);
	void OverlayReset();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimerText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BulletText;

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY()
	class ADuelSheriff* DuelSheriff;

	UPROPERTY(meta = (BindWidget))
	class UButton* LeftButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MiddleButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RightButton;

	UFUNCTION()
	void LeftButtonClicked();

	UFUNCTION()
	void MiddleButtonClicked();

	UFUNCTION()
	void RightButtonClicked();
	
};
