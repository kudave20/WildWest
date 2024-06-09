// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelect.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class WILDWEST_API UCharacterSelect : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GunmanButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SheriffButton;

	void CharacterSelectSetup();
	
protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void GunmanButtonClicked();
	UFUNCTION()
	void SheriffButtonClicked();
};
