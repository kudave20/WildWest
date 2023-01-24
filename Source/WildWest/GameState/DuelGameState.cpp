// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameState.h"

void ADuelGameState::StartDuel()
{
	if (GunmanDuelState == SheriffDuelState)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString(TEXT("Success!"))
			);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Fail!"))
			);
		}
	}
}