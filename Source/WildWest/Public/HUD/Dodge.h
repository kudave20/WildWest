// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dodge.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UDodge : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void DodgeSetup();

	UFUNCTION(BlueprintCallable)
	void DodgeButtonReset();

protected:
	virtual bool Initialize() override;

private:
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
