// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Screen.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Screen;

	UFUNCTION()
	FText SetScreen();
};
