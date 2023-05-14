// Fill out your copyright notice in the Description page of Project Settings.


#include "SheriffOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/GameState/TownGameState.h"
#include "WildWest/Character/Sheriff.h"
#include "GameViewportWidget.h"
#include "Components/Image.h"

void USheriffOverlay::OverlaySetup()
{
	AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ATownGameState* TownGameState = World->GetGameState<ATownGameState>();
	if (TownGameState == nullptr) return;

	TArray<ASheriff*>& SheriffList = TownGameState->GetSheriffList();
	if (SheriffList.IsEmpty()) return;

	if (FirstDeadScreen == nullptr || SecondDeadScreen == nullptr || ThirdDeadScreen == nullptr || FourthDeadScreen == nullptr) return;

	FirstDeadScreen->SetVisibility(ESlateVisibility::Hidden);
	SecondDeadScreen->SetVisibility(ESlateVisibility::Hidden);
	ThirdDeadScreen->SetVisibility(ESlateVisibility::Hidden);
	FourthDeadScreen->SetVisibility(ESlateVisibility::Hidden);

	FVector Location;
	FRotator Rotation;
	if (FirstViewport)
	{
		if (SheriffList[0])
		{
			SheriffList[0]->GetActorEyesViewPoint(Location, Rotation);
			Location += Rotation.Vector() * 10.f;
			FirstViewport->SetCameraLocation(false, Location);
			FirstViewport->SetCameraRotation(false, Rotation);
		}
		else
		{
			FirstViewport->SetVisibility(ESlateVisibility::Hidden);
			FirstDeadScreen->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (SecondViewport)
	{
		if (SheriffList[1])
		{
			SheriffList[1]->GetActorEyesViewPoint(Location, Rotation);
			Location += Rotation.Vector() * 10.f;
			SecondViewport->SetCameraLocation(false, Location);
			SecondViewport->SetCameraRotation(false, Rotation);
		}
		else
		{
			SecondViewport->SetVisibility(ESlateVisibility::Hidden);
			SecondDeadScreen->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (ThirdViewport)
	{
		if (SheriffList[2])
		{
			SheriffList[2]->GetActorEyesViewPoint(Location, Rotation);
			Location += Rotation.Vector() * 10.f;
			ThirdViewport->SetCameraLocation(false, Location);
			ThirdViewport->SetCameraRotation(false, Rotation);
		}
		else
		{
			ThirdViewport->SetVisibility(ESlateVisibility::Hidden);
			ThirdDeadScreen->SetVisibility(ESlateVisibility::Visible);
		}
	}
	if (FourthViewport)
	{
		if (SheriffList[3])
		{
			SheriffList[3]->GetActorEyesViewPoint(Location, Rotation);
			Location += Rotation.Vector() * 10.f;
			FourthViewport->SetCameraLocation(false, Location);
			FourthViewport->SetCameraRotation(false, Rotation);
		}
		else
		{
			FourthViewport->SetVisibility(ESlateVisibility::Hidden);
			FourthDeadScreen->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
