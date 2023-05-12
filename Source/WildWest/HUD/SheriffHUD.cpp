// Fill out your copyright notice in the Description page of Project Settings.


#include "SheriffHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"

void ASheriffHUD::BeginPlay()
{
	Super::BeginPlay();
	
	AddCharacterOverlay();
}

void ASheriffHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}
