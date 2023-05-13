// Fill out your copyright notice in the Description page of Project Settings.


#include "SheriffHUD.h"
#include "GameFramework/PlayerController.h"
#include "SheriffOverlay.h"

void ASheriffHUD::AddSheriffOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && SheriffOverlayClass)
	{
		SheriffOverlay = CreateWidget<USheriffOverlay>(PlayerController, SheriffOverlayClass);
		SheriffOverlay->OverlaySetup();
	}
}
