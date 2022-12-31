// Fill out your copyright notice in the Description page of Project Settings.


#include "GunmanController.h"
#include "WildWest/HUD/CharacterSelect.h"

void AGunmanController::CreateCharacterSelect()
{
	CharacterSelect = CreateWidget<UCharacterSelect>(this, CharacterSelectWidget);
	if (CharacterSelect)
	{
		CharacterSelect->CharacterSelectSetup();
	}
}