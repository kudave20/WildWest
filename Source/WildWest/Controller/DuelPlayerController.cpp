// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelPlayerController.h"
#include "Camera/CameraActor.h"

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

void ADuelPlayerController::ClientSetInputModeGameOnly_Implementation()
{
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	SetShowMouseCursor(false);
}

void ADuelPlayerController::SetInitialControlRotation(const FRotator& NewRotation)
{
	SetControlRotation(NewRotation);
	ClientSetRotation(NewRotation);
}
