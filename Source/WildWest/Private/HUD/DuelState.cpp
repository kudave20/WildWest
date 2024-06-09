// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DuelState.h"
#include "Components/TextBlock.h"
#include "Game/DuelGameState.h"

bool UDuelState::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (TimerText)
	{
		TimerText->TextDelegate.BindUFunction(this, "SetTimerText");
	}
	if (CountText)
	{
		CountText->TextDelegate.BindUFunction(this, "SetCountText");
	}

	return true;
}

FText UDuelState::SetTimerText()
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

FText UDuelState::SetCountText()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
		if (DuelGameState)
		{
			int32 Count = DuelGameState->GetBulletCount();
			return FText::AsNumber(Count);
		}
	}

	return FText();
}
