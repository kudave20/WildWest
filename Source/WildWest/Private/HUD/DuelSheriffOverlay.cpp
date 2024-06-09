// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DuelSheriffOverlay.h"
#include "Components/Button.h"
#include "Character/DuelSheriff.h"

void UDuelSheriffOverlay::OverlaySetup(APlayerController* Controller)
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

		DuelSheriff = Controller->GetPawn<ADuelSheriff>();
	}
}

void UDuelSheriffOverlay::OverlayReset()
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

bool UDuelSheriffOverlay::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (LeftButton)
	{
		LeftButton->OnClicked.AddDynamic(this, &UDuelSheriffOverlay::LeftButtonClicked);
	}
	if (MiddleButton)
	{
		MiddleButton->OnClicked.AddDynamic(this, &UDuelSheriffOverlay::MiddleButtonClicked);
	}
	if (RightButton)
	{
		RightButton->OnClicked.AddDynamic(this, &UDuelSheriffOverlay::RightButtonClicked);
	}

	return true;
}

void UDuelSheriffOverlay::LeftButtonClicked()
{
	LeftButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	DuelSheriff = DuelSheriff == nullptr ? World->GetFirstPlayerController()->GetPawn<ADuelSheriff>() : DuelSheriff;
	if (DuelSheriff)
	{
		DuelSheriff->DodgeLeft();
	}

	MiddleButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UDuelSheriffOverlay::MiddleButtonClicked()
{
	MiddleButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	DuelSheriff = DuelSheriff == nullptr ? World->GetFirstPlayerController()->GetPawn<ADuelSheriff>() : DuelSheriff;
	if (DuelSheriff)
	{
		DuelSheriff->Reload();
	}

	LeftButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UDuelSheriffOverlay::RightButtonClicked()
{
	RightButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	DuelSheriff = DuelSheriff == nullptr ? World->GetFirstPlayerController()->GetPawn<ADuelSheriff>() : DuelSheriff;
	if (DuelSheriff)
	{
		DuelSheriff->DodgeRight();
	}

	LeftButton->SetIsEnabled(true);
	MiddleButton->SetIsEnabled(true);
}
