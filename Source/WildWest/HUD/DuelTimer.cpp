// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelTimer.h"
#include "Components/TextBlock.h"
#include "WildWest/GameState/DuelGameState.h"

bool UDuelTimer::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (TimerText)
	{
		TimerText->TextDelegate.BindUFunction(this, "SetDuelTimer");
	}

	return true;
}

FText UDuelTimer::SetDuelTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			int32 Timer = DuelGameState->GetDuelTimer();
			return FText::AsNumber(Timer);
		}
	}

	return FText();
}
