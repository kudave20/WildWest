// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameState.h"
#include "Net/UnrealNetwork.h"

void ATownGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATownGameState, SheriffList);
}
