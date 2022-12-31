// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelect.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UCharacterSelect : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CharacterSelectSetup();

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* GunmanButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SheriffButton;

	UFUNCTION()
	void GunmanButtonClicked();

	UFUNCTION()
	void SheriffButtonClicked();

	void CharacterSelectTearDown();
};
