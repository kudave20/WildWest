// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriffOverlay.h"
#include "Components/Button.h"
#include "WildWest/Character/DuelSheriff.h"
#include "WildWest/GameState/DuelGameState.h"
#include "WildWest/Controller/DuelPlayerController.h"

void UDuelSheriffOverlay::OverlaySetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputModeData);
		PlayerController->SetShowMouseCursor(true);
	}

	DuelSheriff = World->GetFirstPlayerController()->GetPawn<ADuelSheriff>();
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
