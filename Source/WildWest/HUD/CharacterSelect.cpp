// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect.h"
#include "Components/Button.h"

void UCharacterSelect::CharacterSelectSetup()
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
}

bool UCharacterSelect::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (GunmanButton)
	{
		GunmanButton->OnClicked.AddDynamic(this, &ThisClass::GunmanButtonClicked);
	}
	if (SheriffButton)
	{
		SheriffButton->OnClicked.AddDynamic(this, &ThisClass::SheriffButtonClicked);
	}

	return true;
}

void UCharacterSelect::GunmanButtonClicked()
{
	GunmanButton->SetIsEnabled(false);
}

void UCharacterSelect::SheriffButtonClicked()
{
	SheriffButton->SetIsEnabled(false);
}

void UCharacterSelect::CharacterSelectTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}

void UCharacterSelect::NativeDestruct()
{
	CharacterSelectTearDown();

	Super::NativeDestruct();
}
