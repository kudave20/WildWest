// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DuelState.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class WILDWEST_API UDuelState : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimerText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CountText;

	UFUNCTION()
	FText SetTimerText();

	UFUNCTION()
	FText SetCountText();

};
