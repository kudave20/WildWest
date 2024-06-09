// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Option.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UOption : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OptionSetup();

protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundMix* InMix;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundClass* InSoundClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* FullScreenButton;

	UPROPERTY(meta = (BindWidget))
	UButton* WindowedButton;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* DRComboBox;

	UPROPERTY(meta = (BindWidget))
	UButton* FPSLowButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FPSMediumButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FPSHighButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FPSEpicButton;

	UPROPERTY(meta = (BindWidget))
	UButton* VDLowButton;

	UPROPERTY(meta = (BindWidget))
	UButton* VDMediumButton;

	UPROPERTY(meta = (BindWidget))
	UButton* VDHighButton;

	UPROPERTY(meta = (BindWidget))
	UButton* VDEpicButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PPLowButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PPMediumButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PPHighButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PPEpicButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AALowButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AAMediumButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AAHighButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AAEpicButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TQLowButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TQMediumButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TQHighButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TQEpicButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SQLowButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SQMediumButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SQHighButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SQEpicButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OptimalButton;

	UPROPERTY(meta = (BindWidget))
	class USlider* VolumeSlider;

	UFUNCTION()
	void FullScreenButtonClicked();

	UFUNCTION()
	void WindowedButtonClicked();

	UFUNCTION()
	void DRComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void FPSLowButtonClicked();

	UFUNCTION()
	void FPSMediumButtonClicked();

	UFUNCTION()
	void FPSHighButtonClicked();

	UFUNCTION()
	void FPSEpicButtonClicked();

	UFUNCTION()
	void VDLowButtonClicked();

	UFUNCTION()
	void VDMediumButtonClicked();

	UFUNCTION()
	void VDHighButtonClicked();

	UFUNCTION()
	void VDEpicButtonClicked();

	UFUNCTION()
	void PPLowButtonClicked();

	UFUNCTION()
	void PPMediumButtonClicked();

	UFUNCTION()
	void PPHighButtonClicked();

	UFUNCTION()
	void PPEpicButtonClicked();

	UFUNCTION()
	void AALowButtonClicked();

	UFUNCTION()
	void AAMediumButtonClicked();

	UFUNCTION()
	void AAHighButtonClicked();

	UFUNCTION()
	void AAEpicButtonClicked();

	UFUNCTION()
	void TQLowButtonClicked();

	UFUNCTION()
	void TQMediumButtonClicked();

	UFUNCTION()
	void TQHighButtonClicked();

	UFUNCTION()
	void TQEpicButtonClicked();

	UFUNCTION()
	void SQLowButtonClicked();

	UFUNCTION()
	void SQMediumButtonClicked();

	UFUNCTION()
	void SQHighButtonClicked();

	UFUNCTION()
	void SQEpicButtonClicked();

	UFUNCTION()
	void BackButtonClicked();

	UFUNCTION()
	void OptimalButtonClicked();

	UFUNCTION()
	void VolumeSliderValueChanged(float Value);

	void SelectWindowMode(bool bFullScreen, bool bWindowed);
	void SelectFPSButton(bool bFPSLow, bool bFPSMedium, bool bFPSHigh, bool bFPSEpic);
	void SelectViewDistanceButton(bool bVDLow, bool bVDMedium, bool bVDHigh, bool bVDEpic);
	void SelectPostProcessingButton(bool bPPLow, bool bPPMedium, bool bPPHigh, bool bPPEpic);
	void SelectAntiAliasingButton(bool bAALow, bool bAAMedium, bool bAAHigh, bool bAAEpic);
	void SelectTextureQualityButton(bool bTQLow, bool bTQMedium, bool bTQHigh, bool bTQEpic);
	void SelectShadowQualityButton(bool bSQLow, bool bSQMedium, bool bSQHigh, bool bSQEpic);

	void OptionButtonSetup();

};
