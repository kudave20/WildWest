// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Option.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Game/WildWestGameInstance.h"

void UOption::OptionSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	OptionButtonSetup();
}

bool UOption::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (FullScreenButton)
	{
		FullScreenButton->OnClicked.AddDynamic(this, &ThisClass::FullScreenButtonClicked);
	}
	if (WindowedButton)
	{
		WindowedButton->OnClicked.AddDynamic(this, &ThisClass::WindowedButtonClicked);
	}
	if (DRComboBox)
	{
		DRComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::DRComboBoxSelectionChanged);
	}
	if (FPSLowButton)
	{
		FPSLowButton->OnClicked.AddDynamic(this, &ThisClass::FPSLowButtonClicked);
	}
	if (FPSMediumButton)
	{
		FPSMediumButton->OnClicked.AddDynamic(this, &ThisClass::FPSMediumButtonClicked);
	}
	if (FPSHighButton)
	{
		FPSHighButton->OnClicked.AddDynamic(this, &ThisClass::FPSHighButtonClicked);
	}
	if (FPSEpicButton)
	{
		FPSEpicButton->OnClicked.AddDynamic(this, &ThisClass::FPSEpicButtonClicked);
	}
	if (VDLowButton)
	{
		VDLowButton->OnClicked.AddDynamic(this, &ThisClass::VDLowButtonClicked);
	}
	if (VDMediumButton)
	{
		VDMediumButton->OnClicked.AddDynamic(this, &ThisClass::VDMediumButtonClicked);
	}
	if (VDHighButton)
	{
		VDHighButton->OnClicked.AddDynamic(this, &ThisClass::VDHighButtonClicked);
	}
	if (VDEpicButton)
	{
		VDEpicButton->OnClicked.AddDynamic(this, &ThisClass::VDEpicButtonClicked);
	}
	if (PPLowButton)
	{
		PPLowButton->OnClicked.AddDynamic(this, &ThisClass::PPLowButtonClicked);
	}
	if (PPMediumButton)
	{
		PPMediumButton->OnClicked.AddDynamic(this, &ThisClass::PPMediumButtonClicked);
	}
	if (PPHighButton)
	{
		PPHighButton->OnClicked.AddDynamic(this, &ThisClass::PPHighButtonClicked);
	}
	if (PPEpicButton)
	{
		PPEpicButton->OnClicked.AddDynamic(this, &ThisClass::PPEpicButtonClicked);
	}
	if (AALowButton)
	{
		AALowButton->OnClicked.AddDynamic(this, &ThisClass::AALowButtonClicked);
	}
	if (AAMediumButton)
	{
		AAMediumButton->OnClicked.AddDynamic(this, &ThisClass::AAMediumButtonClicked);
	}
	if (AAHighButton)
	{
		AAHighButton->OnClicked.AddDynamic(this, &ThisClass::AAHighButtonClicked);
	}
	if (AAEpicButton)
	{
		AAEpicButton->OnClicked.AddDynamic(this, &ThisClass::AAEpicButtonClicked);
	}
	if (TQLowButton)
	{
		TQLowButton->OnClicked.AddDynamic(this, &ThisClass::TQLowButtonClicked);
	}
	if (TQMediumButton)
	{
		TQMediumButton->OnClicked.AddDynamic(this, &ThisClass::TQMediumButtonClicked);
	}
	if (TQHighButton)
	{
		TQHighButton->OnClicked.AddDynamic(this, &ThisClass::TQHighButtonClicked);
	}
	if (TQEpicButton)
	{
		TQEpicButton->OnClicked.AddDynamic(this, &ThisClass::TQEpicButtonClicked);
	}
	if (SQLowButton)
	{
		SQLowButton->OnClicked.AddDynamic(this, &ThisClass::SQLowButtonClicked);
	}
	if (SQMediumButton)
	{
		SQMediumButton->OnClicked.AddDynamic(this, &ThisClass::SQMediumButtonClicked);
	}
	if (SQHighButton)
	{
		SQHighButton->OnClicked.AddDynamic(this, &ThisClass::SQHighButtonClicked);
	}
	if (SQEpicButton)
	{
		SQEpicButton->OnClicked.AddDynamic(this, &ThisClass::SQEpicButtonClicked);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
	}
	if (OptimalButton)
	{
		OptimalButton->OnClicked.AddDynamic(this, &ThisClass::OptimalButtonClicked);
	}
	if (VolumeSlider)
	{
		VolumeSlider->OnValueChanged.AddDynamic(this, &ThisClass::VolumeSliderValueChanged);
	}

	return true;
}

void UOption::FullScreenButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
			UserSettings->ApplySettings(false);
			SelectWindowMode(true, false);
		}
	}
}

void UOption::WindowedButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFullscreenMode(EWindowMode::Windowed);
			UserSettings->ApplySettings(false);
			SelectWindowMode(false, true);
		}
	}
}

void UOption::DRComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			FString LeftString, RightString;
			SelectedItem.Split(FString("x"), &LeftString, &RightString);

			int32 ResX = FCString::Atoi(*LeftString);
			int32 ResY = FCString::Atoi(*RightString);

			FIntPoint Resolution(ResX, ResY);
			UserSettings->SetScreenResolution(Resolution);
			UserSettings->ApplySettings(false);
		}
	}
}

void UOption::FPSLowButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFrameRateLimit(30.0f);
			UserSettings->ApplySettings(false);
			SelectFPSButton(true, false, false, false);
		}
	}
}

void UOption::FPSMediumButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFrameRateLimit(60.0f);
			UserSettings->ApplySettings(false);
			SelectFPSButton(false, true, false, false);
		}
	}
}

void UOption::FPSHighButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFrameRateLimit(144.0f);
			UserSettings->ApplySettings(false);
			SelectFPSButton(false, false, true, false);
		}
	}
}

void UOption::FPSEpicButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetFrameRateLimit(0.0f);
			UserSettings->ApplySettings(false);
			SelectFPSButton(false, false, false, true);
		}
	}
}

void UOption::VDLowButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetViewDistanceQuality(0);
			UserSettings->ApplySettings(false);
			SelectViewDistanceButton(true, false, false, false);
		}
	}
}

void UOption::VDMediumButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetViewDistanceQuality(1);
			UserSettings->ApplySettings(false);
			SelectViewDistanceButton(false, true, false, false);
		}
	}
}

void UOption::VDHighButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetViewDistanceQuality(2);
			UserSettings->ApplySettings(false);
			SelectViewDistanceButton(false, false, true, false);
		}
	}
}

void UOption::VDEpicButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetViewDistanceQuality(3);
			UserSettings->ApplySettings(false);
			SelectViewDistanceButton(false, false, false, true);
		}
	}
}

void UOption::PPLowButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetPostProcessingQuality(0);
			UserSettings->ApplySettings(false);
			SelectPostProcessingButton(true, false, false, false);
		}
	}
}

void UOption::PPMediumButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetPostProcessingQuality(1);
			UserSettings->ApplySettings(false);
			SelectPostProcessingButton(false, true, false, false);
		}
	}
}

void UOption::PPHighButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetPostProcessingQuality(2);
			UserSettings->ApplySettings(false);
			SelectPostProcessingButton(false, false, true, false);
		}
	}
}

void UOption::PPEpicButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetPostProcessingQuality(3);
			UserSettings->ApplySettings(false);
			SelectPostProcessingButton(false, false, false, true);
		}
	}
}

void UOption::AALowButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetAntiAliasingQuality(0);
			UserSettings->ApplySettings(false);
			SelectAntiAliasingButton(true, false, false, false);
		}
	}
}

void UOption::AAMediumButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetAntiAliasingQuality(1);
			UserSettings->ApplySettings(false);
			SelectAntiAliasingButton(false, true, false, false);
		}
	}
}

void UOption::AAHighButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetAntiAliasingQuality(2);
			UserSettings->ApplySettings(false);
			SelectAntiAliasingButton(false, false, true, false);
		}
	}
}

void UOption::AAEpicButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetAntiAliasingQuality(3);
			UserSettings->ApplySettings(false);
			SelectAntiAliasingButton(false, false, false, true);
		}
	}
}

void UOption::TQLowButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetTextureQuality(0);
			UserSettings->ApplySettings(false);
			SelectTextureQualityButton(true, false, false, false);
		}
	}
}

void UOption::TQMediumButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetTextureQuality(1);
			UserSettings->ApplySettings(false);
			SelectTextureQualityButton(false, true, false, false);
		}
	}
}

void UOption::TQHighButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetTextureQuality(2);
			UserSettings->ApplySettings(false);
			SelectTextureQualityButton(false, false, true, false);
		}
	}
}

void UOption::TQEpicButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetTextureQuality(3);
			UserSettings->ApplySettings(false);
			SelectTextureQualityButton(false, false, false, true);
		}
	}
}

void UOption::SQLowButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetShadowQuality(0);
			UserSettings->ApplySettings(false);
			SelectShadowQualityButton(true, false, false, false);
		}
	}
}

void UOption::SQMediumButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetShadowQuality(1);
			UserSettings->ApplySettings(false);
			SelectShadowQualityButton(false, true, false, false);
		}
	}
}

void UOption::SQHighButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetShadowQuality(2);
			UserSettings->ApplySettings(false);
			SelectShadowQualityButton(false, false, true, false);
		}
	}
}

void UOption::SQEpicButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->SetShadowQuality(3);
			UserSettings->ApplySettings(false);
			SelectShadowQualityButton(false, false, false, true);
		}
	}
}

void UOption::BackButtonClicked()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance && VolumeSlider)
	{
		WildWestGameInstance->SetMasterVolume(VolumeSlider->GetValue());
	}
}

void UOption::OptimalButtonClicked()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			UserSettings->RunHardwareBenchmark();
			UserSettings->ApplyHardwareBenchmarkResults();
			OptionButtonSetup();
		}
	}
}

void UOption::VolumeSliderValueChanged(float Value)
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::SetSoundMixClassOverride(World, InMix, InSoundClass, Value);
		UGameplayStatics::PushSoundMixModifier(World, InMix);
	}
}

void UOption::SelectWindowMode(bool bFullScreen, bool bWindowed)
{
	if (FullScreenButton)
	{
		FullScreenButton->SetIsEnabled(!bFullScreen);
	}
	if (WindowedButton)
	{
		WindowedButton->SetIsEnabled(!bWindowed);
	}
}

void UOption::SelectFPSButton(bool bFPSLow, bool bFPSMedium, bool bFPSHigh, bool bFPSEpic)
{
	if (FPSLowButton)
	{
		FPSLowButton->SetIsEnabled(!bFPSLow);
	}
	if (FPSMediumButton)
	{
		FPSMediumButton->SetIsEnabled(!bFPSMedium);
	}
	if (FPSHighButton)
	{
		FPSHighButton->SetIsEnabled(!bFPSHigh);
	}
	if (FPSEpicButton)
	{
		FPSEpicButton->SetIsEnabled(!bFPSEpic);
	}
}

void UOption::SelectViewDistanceButton(bool bVDLow, bool bVDMedium, bool bVDHigh, bool bVDEpic)
{
	if (VDLowButton)
	{
		VDLowButton->SetIsEnabled(!bVDLow);
	}
	if (VDMediumButton)
	{
		VDMediumButton->SetIsEnabled(!bVDMedium);
	}
	if (VDHighButton)
	{
		VDHighButton->SetIsEnabled(!bVDHigh);
	}
	if (VDEpicButton)
	{
		VDEpicButton->SetIsEnabled(!bVDEpic);
	}
}

void UOption::SelectPostProcessingButton(bool bPPLow, bool bPPMedium, bool bPPHigh, bool bPPEpic)
{
	if (PPLowButton)
	{
		PPLowButton->SetIsEnabled(!bPPLow);
	}
	if (PPMediumButton)
	{
		PPMediumButton->SetIsEnabled(!bPPMedium);
	}
	if (PPHighButton)
	{
		PPHighButton->SetIsEnabled(!bPPHigh);
	}
	if (PPEpicButton)
	{
		PPEpicButton->SetIsEnabled(!bPPEpic);
	}
}

void UOption::SelectAntiAliasingButton(bool bAALow, bool bAAMedium, bool bAAHigh, bool bAAEpic)
{
	if (AALowButton)
	{
		AALowButton->SetIsEnabled(!bAALow);
	}
	if (AAMediumButton)
	{
		AAMediumButton->SetIsEnabled(!bAAMedium);
	}
	if (AAHighButton)
	{
		AAHighButton->SetIsEnabled(!bAAHigh);
	}
	if (AAEpicButton)
	{
		AAEpicButton->SetIsEnabled(!bAAEpic);
	}
}

void UOption::SelectTextureQualityButton(bool bTQLow, bool bTQMedium, bool bTQHigh, bool bTQEpic)
{
	if (TQLowButton)
	{
		TQLowButton->SetIsEnabled(!bTQLow);
	}
	if (TQMediumButton)
	{
		TQMediumButton->SetIsEnabled(!bTQMedium);
	}
	if (TQHighButton)
	{
		TQHighButton->SetIsEnabled(!bTQHigh);
	}
	if (TQEpicButton)
	{
		TQEpicButton->SetIsEnabled(!bTQEpic);
	}
}

void UOption::SelectShadowQualityButton(bool bSQLow, bool bSQMedium, bool bSQHigh, bool bSQEpic)
{
	if (SQLowButton)
	{
		SQLowButton->SetIsEnabled(!bSQLow);
	}
	if (SQMediumButton)
	{
		SQMediumButton->SetIsEnabled(!bSQMedium);
	}
	if (SQHighButton)
	{
		SQHighButton->SetIsEnabled(!bSQHigh);
	}
	if (SQEpicButton)
	{
		SQEpicButton->SetIsEnabled(!bSQEpic);
	}
}

void UOption::OptionButtonSetup()
{
	if (GEngine)
	{
		UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
		if (UserSettings)
		{
			switch (UserSettings->GetFullscreenMode())
			{
			case EWindowMode::Fullscreen:
				SelectWindowMode(true, false);
				break;
			case EWindowMode::Windowed:
				SelectWindowMode(false, true);
				break;
			}

			if (DRComboBox)
			{
				switch (UserSettings->GetScreenResolution().Y)
				{
				case 480:
					DRComboBox->SetSelectedOption(FString("720x480"));
					break;
				case 720:
					DRComboBox->SetSelectedOption(FString("1280x720"));
					break;
				case 1080:
					DRComboBox->SetSelectedOption(FString("1920x1080"));
					break;
				case 1440:
					DRComboBox->SetSelectedOption(FString("2560x1440"));
					break;
				}
			}

			switch (int32 FrameRateLimit = UserSettings->GetFrameRateLimit())
			{
			case 30:
				SelectFPSButton(true, false, false, false);
				break;
			case 60:
				SelectFPSButton(false, true, false, false);
				break;
			case 144:
				SelectFPSButton(false, false, true, false);
				break;
			case 0:
				SelectFPSButton(false, false, false, true);
				break;
			}

			switch (UserSettings->GetViewDistanceQuality())
			{
			case 0:
				SelectViewDistanceButton(true, false, false, false);
				break;
			case 1:
				SelectViewDistanceButton(false, true, false, false);
				break;
			case 2:
				SelectViewDistanceButton(false, false, true, false);
				break;
			case 3:
				SelectViewDistanceButton(false, false, false, true);
				break;
			}

			switch (UserSettings->GetPostProcessingQuality())
			{
			case 0:
				SelectPostProcessingButton(true, false, false, false);
				break;
			case 1:
				SelectPostProcessingButton(false, true, false, false);
				break;
			case 2:
				SelectPostProcessingButton(false, false, true, false);
				break;
			case 3:
				SelectPostProcessingButton(false, false, false, true);
				break;
			}

			switch (UserSettings->GetAntiAliasingQuality())
			{
			case 0:
				SelectAntiAliasingButton(true, false, false, false);
				break;
			case 1:
				SelectAntiAliasingButton(false, true, false, false);
				break;
			case 2:
				SelectAntiAliasingButton(false, false, true, false);
				break;
			case 3:
				SelectAntiAliasingButton(false, false, false, true);
				break;
			}

			switch (UserSettings->GetTextureQuality())
			{
			case 0:
				SelectTextureQualityButton(true, false, false, false);
				break;
			case 1:
				SelectTextureQualityButton(false, true, false, false);
				break;
			case 2:
				SelectTextureQualityButton(false, false, true, false);
				break;
			case 3:
				SelectTextureQualityButton(false, false, false, true);
				break;
			}

			switch (UserSettings->GetShadowQuality())
			{
			case 0:
				SelectShadowQualityButton(true, false, false, false);
				break;
			case 1:
				SelectShadowQualityButton(false, true, false, false);
				break;
			case 2:
				SelectShadowQualityButton(false, false, true, false);
				break;
			case 3:
				SelectShadowQualityButton(false, false, false, true);
				break;
			}
		}
	}

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance && VolumeSlider)
	{
		VolumeSlider->SetValue(WildWestGameInstance->GetMasterVolume());
	}
}
