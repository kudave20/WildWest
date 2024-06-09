// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SheriffOverlay.h"
#include "Components/Image.h"
#include "Game/TownGameState.h"
#include "HUD/GameViewportWidget.h"

void USheriffOverlay::OverlaySetup()
{
	AddToViewport();

	ATownGameState* TownGameState = GetWorld()->GetGameState<ATownGameState>();
	if (TownGameState == nullptr || TownGameState->SheriffList.IsEmpty()) return;

	if (Viewport)
	{
		Viewport->SetVisibility(ESlateVisibility::Hidden);
		Viewport->SetVisibility(ESlateVisibility::Hidden);
	}
}
