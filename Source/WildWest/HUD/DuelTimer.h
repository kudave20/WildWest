// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DuelTimer.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UDuelTimer : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimerText;

	UFUNCTION()
	FText SetDuelTimer();

};
