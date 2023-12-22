// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriffHUD.h"
#include "GameFramework/PlayerController.h"
#include "DuelSheriffOverlay.h"

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