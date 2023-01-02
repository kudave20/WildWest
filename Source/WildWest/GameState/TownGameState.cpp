// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameState.h"
#include "WildWest/Character/Sheriff.h"
#include "EngineUtils.h"

void ATownGameState::SheriffListSetup()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<ASheriff> Iter(World); Iter; ++Iter)
		{
			ASheriff* Sheriff = *Iter;
			SheriffList.Add(Sheriff);
		}
	}
}
