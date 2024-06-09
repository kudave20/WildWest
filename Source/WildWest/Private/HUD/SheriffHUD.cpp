// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SheriffHUD.h"
#include "GameFramework/PlayerController.h"
#include "HUD/SheriffOverlay.h"

void ASheriffHUD::AddSheriffOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && SheriffOverlayClass)
	{
		SheriffOverlay = CreateWidget<USheriffOverlay>(PlayerController, SheriffOverlayClass);
		SheriffOverlay->OverlaySetup();
	}
}
