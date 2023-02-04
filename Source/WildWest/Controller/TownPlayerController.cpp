// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"

void ATownPlayerController::SetInitialControlRotation(const FRotator& NewRotation)
{
	SetControlRotation(NewRotation);
	ClientSetRotation(NewRotation);
}

void ATownPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	if (!IsLocalPlayerController())
	{
		ClientSetInputModeGameOnly();
		return;
	}

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	SetShowMouseCursor(false);
}

void ATownPlayerController::ClientSetInputModeGameOnly_Implementation()
{
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	SetShowMouseCursor(false);
}
