// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"

void ATownPlayerController::SetInitialControlRotation(const FRotator& NewRotation)
{
	SetControlRotation(NewRotation);
	ClientSetRotation(NewRotation);
}

void ATownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	SetShowMouseCursor(false);

	UWorld* World = GetWorld();
	if (World)
	{
		UGameViewportClient* GameViewportClient = World->GetGameViewport();
		if (GameViewportClient)
		{
			GameViewportClient->SetForceDisableSplitscreen(false);
		}
	}
}
