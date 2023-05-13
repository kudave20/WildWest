// Fill out your copyright notice in the Description page of Project Settings.


#include "SheriffOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/Character/Sheriff.h"
#include "GameViewportWidget.h"

void USheriffOverlay::OverlaySetup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
	if (TownGameState == nullptr) return;

	TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();
	if (SheriffList.IsEmpty()) return;

	FVector Location;
	FRotator Rotation;
	if (FirstViewport)
	{
		SheriffList[0]->GetActorEyesViewPoint(Location, Rotation);
		FirstViewport->SetCameraLocation(false, Location);
		FirstViewport->SetCameraRotation(false, Rotation);
	}
	if (SecondViewport)
	{
		SheriffList[1]->GetActorEyesViewPoint(Location, Rotation);
		SecondViewport->SetCameraLocation(false, Location);
		SecondViewport->SetCameraRotation(false, Rotation);
	}
	if (ThirdViewport)
	{
		SheriffList[2]->GetActorEyesViewPoint(Location, Rotation);
		ThirdViewport->SetCameraLocation(false, Location);
		ThirdViewport->SetCameraRotation(false, Rotation);
	}
	if (FourthViewport && SheriffList.Num() >= 4)
	{
		SheriffList[3]->GetActorEyesViewPoint(Location, Rotation);
		FourthViewport->SetCameraLocation(false, Location);
		FourthViewport->SetCameraRotation(false, Rotation);
	}
	
	return;
}
