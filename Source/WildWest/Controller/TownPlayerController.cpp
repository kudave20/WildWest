// Fill out your copyright notice in the Description page of Project Settings.


#include "TownPlayerController.h"
#include "Net/UnrealNetwork.h"

void ATownPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATownPlayerController, bIsPossessed);
}

void ATownPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	bIsPossessed = true;
}

void ATownPlayerController::OnRep_bIsPossessed()
{
	PossessCompleteDelegate.Broadcast();
}
