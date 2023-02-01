// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelPlayerController.h"
#include "Kismet/GameplayStatics.h"
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

void ADuelPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UWorld* World = GetWorld();
	if (World)
	{
		AActor* CameraActor = UGameplayStatics::GetActorOfClass(World, ACameraActor::StaticClass());
		SetViewTargetWithBlend(CameraActor);
	}
}