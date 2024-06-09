// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DuelGunmanOverlay.h"
#include "Components/Button.h"
#include "Character/DuelGunman.h"

void UDuelGunmanOverlay::OverlaySetup(APlayerController* Controller)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	if (Controller)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Controller->SetInputMode(InputModeData);
		Controller->SetShowMouseCursor(true);

		DuelGunman = Controller->GetPawn<ADuelGunman>();
	}
}

void UDuelGunmanOverlay::OverlayReset()
{
	if (LeftButton)
	{
		LeftButton->SetIsEnabled(true);
	}
	if (MiddleButton)
	{
		MiddleButton->SetIsEnabled(true);
	}
	if (RightButton)
	{
		RightButton->SetIsEnabled(true);
	}
}

bool UDuelGunmanOverlay::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (LeftButton)
	{
		LeftButton->OnClicked.AddDynamic(this, &UDuelGunmanOverlay::LeftButtonClicked);
	}
	if (MiddleButton)
	{
		MiddleButton->OnClicked.AddDynamic(this, &UDuelGunmanOverlay::MiddleButtonClicked);
	}
	if (RightButton)
	{
		RightButton->OnClicked.AddDynamic(this, &UDuelGunmanOverlay::RightButtonClicked);
	}

	return true;
}

void UDuelGunmanOverlay::LeftButtonClicked()
{
	LeftButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	DuelGunman = DuelGunman == nullptr ? World->GetFirstPlayerController()->GetPawn<ADuelGunman>() : DuelGunman;
	if (DuelGunman)
	{
		DuelGunman->ShootLeft();
	}

	MiddleButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UDuelGunmanOverlay::MiddleButtonClicked()
{
	MiddleButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	DuelGunman = DuelGunman == nullptr ? World->GetFirstPlayerController()->GetPawn<ADuelGunman>() : DuelGunman;
	if (DuelGunman)
	{
		DuelGunman->ShootMiddle();
	}

	LeftButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UDuelGunmanOverlay::RightButtonClicked()
{
	RightButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	DuelGunman = DuelGunman == nullptr ? World->GetFirstPlayerController()->GetPawn<ADuelGunman>() : DuelGunman;
	if (DuelGunman)
	{
		DuelGunman->ShootRight();
	}

	LeftButton->SetIsEnabled(true);
	MiddleButton->SetIsEnabled(true);
}
