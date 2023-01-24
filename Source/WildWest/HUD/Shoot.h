// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shoot.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UShoot : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ShootSetup();
	
protected:
	virtual bool Initialize() override;

private:
	class ADuelGunman* DuelGunman;

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
