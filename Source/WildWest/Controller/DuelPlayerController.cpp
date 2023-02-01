// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Blueprint/WidgetLayoutLibrary.h"

ADuelPlayerController::ADuelPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void ADuelPlayerController::ClientHitBroadcast_Implementation()
{
	HitDelegate.Broadcast();
}

void ADuelPlayerController::ClientDodgeBroadcast_Implementation()
{
	DodgeDelegate.Broadcast();
}

void ADuelPlayerController::ClientFireBroadcast_Implementation()
{
	FireDelegate.Broadcast();
}

void ADuelPlayerController::ClientDuelSelectCompleteBroadcast_Implementation()
{
	DuelSelectCompleteDelegate.Broadcast();
}

void ADuelPlayerController::ClientFireCompleteBroadcast_Implementation()
{
	FireCompleteDelegate.Broadcast();
}

void ADuelPlayerController::ClientFightBackBroadcast_Implementation()
{
	FightBackDelegate.Broadcast();
}

void ADuelPlayerController::SetInitialControlRotation(const FRotator& NewRotation)
{
	SetControlRotation(NewRotation);
	ClientSetRotation(NewRotation);
}

void ADuelPlayerController::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();

	UWorld* World = GetWorld();
	if (World)
	{
		UWidgetLayoutLibrary::RemoveAllWidgets(World);
	}
}
