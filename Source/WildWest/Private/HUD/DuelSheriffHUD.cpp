// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DuelSheriffHUD.h"
#include "GameFramework/PlayerController.h"
#include "HUD/DuelSheriffOverlay.h"

void ADuelSheriffHUD::AddDuelSheriffOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && DuelSheriffOverlayClass)
	{
		DuelSheriffOverlay = CreateWidget<UDuelSheriffOverlay>(PlayerController, DuelSheriffOverlayClass);
		DuelSheriffOverlay->OverlaySetup(PlayerController);
	}
}

void ADuelSheriffHUD::RemoveOverlay()
{
	if (DuelSheriffOverlay)
	{
		DuelSheriffOverlay->RemoveFromParent();
	}
}
