// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGunmanHUD.h"
#include "GameFramework/PlayerController.h"
#include "DuelGunmanOverlay.h"

void ADuelGunmanHUD::AddDuelGunmanOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && DuelGunmanOverlayClass)
	{
		DuelGunmanOverlay = CreateWidget<UDuelGunmanOverlay>(PlayerController, DuelGunmanOverlayClass);
		DuelGunmanOverlay->OverlaySetup(PlayerController);
	}
}

void ADuelGunmanHUD::RemoveOverlay()
{
	if (DuelGunmanOverlay)
	{
		DuelGunmanOverlay->RemoveFromParent();
	}
}
