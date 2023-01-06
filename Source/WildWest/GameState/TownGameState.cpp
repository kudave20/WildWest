// Fill out your copyright notice in the Description page of Project Settings.


#include "TownGameState.h"
#include "WildWest/Character/Sheriff.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void ATownGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATownGameState, SheriffList);
	DOREPLIFETIME(ATownGameState, LastScreenIndex);
	DOREPLIFETIME(ATownGameState, CurrentScreenIndex);
}


void ATownGameState::SheriffListSetup()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (ASheriff* Sheriff : TActorRange<ASheriff>(World))
		{
			SheriffList.Add(Sheriff);
		}
	}
}

void ATownGameState::OnRep_SheriffList()
{
	SheriffListReplicateCompleteDelegate.Broadcast();
}

void ATownGameState::OnRep_CurrentScreenIndex()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ASheriff* Sheriff = Cast<ASheriff>(UGameplayStatics::GetPlayerPawn(World, 0));
		if (Sheriff)
		{
			switch (CurrentScreenIndex)
			{
			case EScreenIndex::ECI_First:
				Sheriff->SelectFirstScreenCompleteDelegate.Broadcast(LastScreenIndex, CurrentScreenIndex);
				break;
			case EScreenIndex::ECI_Second:
				Sheriff->SelectSecondScreenCompleteDelegate.Broadcast(LastScreenIndex, CurrentScreenIndex);
				break;
			case EScreenIndex::ECI_Third:
				Sheriff->SelectThirdScreenCompleteDelegate.Broadcast(LastScreenIndex, CurrentScreenIndex);
				break;
			case EScreenIndex::ECI_Fourth:
				Sheriff->SelectFourthScreenCompleteDelegate.Broadcast(LastScreenIndex, CurrentScreenIndex);
				break;
			}
		}
	}
}
