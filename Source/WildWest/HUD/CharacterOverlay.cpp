// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/Character/Sheriff.h"
#include "GameViewportWidget.h"

bool UCharacterOverlay::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	UGameplayStatics::GetAllActorsOfClass(this, ASheriff::StaticClass(), SheriffList);
	return true;
}

void UCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

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
	if (FourthViewport)
	{
		SheriffList[3]->GetActorEyesViewPoint(Location, Rotation);
		FourthViewport->SetCameraLocation(false, Location);
		FourthViewport->SetCameraRotation(false, Rotation);
	}
}