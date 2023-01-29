// Fill out your copyright notice in the Description page of Project Settings.


#include "Dodge.h"
#include "Components/Button.h"
#include "WildWest/Character/DuelSheriff.h"
#include "WildWest/GameState/DuelGameState.h"

void UDodge::DodgeSetup()
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

void UDodge::DodgeButtonReset()
{
	if (LeftButton && !LeftButton->GetIsEnabled())
	{
		LeftButton->SetIsEnabled(true);
	}
	if (MiddleButton && !MiddleButton->GetIsEnabled())
	{
		MiddleButton->SetIsEnabled(true);
	}
	if (RightButton && !RightButton->GetIsEnabled())
	{
		RightButton->SetIsEnabled(true);
	}
}

bool UDodge::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	DuelSheriff = Cast<ADuelSheriff>(GetOwningPlayerPawn());

	if (LeftButton)
	{
		LeftButton->OnClicked.AddDynamic(this, &ThisClass::LeftButtonClicked);
	}
	if (MiddleButton)
	{
		MiddleButton->OnClicked.AddDynamic(this, &ThisClass::MiddleButtonClicked);
	}
	if (RightButton)
	{
		RightButton->OnClicked.AddDynamic(this, &ThisClass::RightButtonClicked);
	}

	return true;
}

void UDodge::LeftButtonClicked()
{
	LeftButton->SetIsEnabled(false);

	if (DuelSheriff)
	{
		DuelSheriff->DodgeLeft();
	}

	MiddleButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UDodge::MiddleButtonClicked()
{
	MiddleButton->SetIsEnabled(false);

	if (DuelSheriff)
	{
		DuelSheriff->Reload();
	}

	LeftButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UDodge::RightButtonClicked()
{
	RightButton->SetIsEnabled(false);

	if (DuelSheriff)
	{
		DuelSheriff->DodgeRight();
	}

	LeftButton->SetIsEnabled(true);
	MiddleButton->SetIsEnabled(true);
}
