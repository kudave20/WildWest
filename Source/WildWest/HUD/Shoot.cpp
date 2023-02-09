// Fill out your copyright notice in the Description page of Project Settings.


#include "Shoot.h"
#include "Components/Button.h"
#include "WildWest/Character/DuelGunman.h"
#include "WildWest/GameState/DuelGameState.h"
#include "WildWest/Controller/DuelPlayerController.h"

void UShoot::ShootSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			ADuelPlayerController* DuelPlayerController = NULL;
			if (World->IsNetMode(ENetMode::NM_ListenServer))
			{
				DuelPlayerController = DuelGameState->GetServerPlayerController();
			}
			else
			{
				DuelPlayerController = DuelGameState->GetClientPlayerController();
			}

			if (DuelPlayerController)
			{
				FInputModeUIOnly InputModeData;
				InputModeData.SetWidgetToFocus(TakeWidget());
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				DuelPlayerController->SetInputMode(InputModeData);
				DuelPlayerController->SetShowMouseCursor(true);
			}
		}
	}
}

void UShoot::ShootButtonReset()
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

bool UShoot::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	DuelGunman = Cast<ADuelGunman>(GetOwningPlayerPawn());

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

void UShoot::LeftButtonClicked()
{
	LeftButton->SetIsEnabled(false);

	if (DuelGunman)
	{
		DuelGunman->ShootLeft();
	}

	MiddleButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UShoot::MiddleButtonClicked()
{
	MiddleButton->SetIsEnabled(false);

	if (DuelGunman)
	{
		DuelGunman->ShootMiddle();
	}

	LeftButton->SetIsEnabled(true);
	RightButton->SetIsEnabled(true);
}

void UShoot::RightButtonClicked()
{
	RightButton->SetIsEnabled(false);

	if (DuelGunman)
	{
		DuelGunman->ShootRight();
	}

	LeftButton->SetIsEnabled(true);
	MiddleButton->SetIsEnabled(true);
}
