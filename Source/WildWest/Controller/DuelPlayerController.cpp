// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelPlayerController.h"
#include "Camera/CameraActor.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "WildWest/GameState/DuelGameState.h"

ADuelPlayerController::ADuelPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void ADuelPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		UGameViewportClient* GameViewportClient = World->GetGameViewport();
		if (GameViewportClient)
		{
			GameViewportClient->SetForceDisableSplitscreen(true);
		}
	}
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

void ADuelPlayerController::SetControllerIndex()
{
	if (!HasAuthority())
	{
		ServerSetControllerIndex();
		return;
	}

	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		int32 Index = -1;
		TArray<int32>& RemovedControllerIndex = WildWestGameInstance->GetRemovedControllerIndex();
		int32 CurrentSheriffIndex = WildWestGameInstance->GetCurrentSheriffIndex();

		for (int32 Idx = 0; Idx < RemovedControllerIndex.Num(); Idx++)
		{
			if (RemovedControllerIndex[Idx] > CurrentSheriffIndex)
			{
				Index = Idx;
				break;
			}
		}

		if (Index != -1)
		{
			RemovedControllerIndex.Insert(CurrentSheriffIndex, Index);
		}
		else
		{
			RemovedControllerIndex.AddUnique(CurrentSheriffIndex);
		}

		TArray<int32>& AliveControllerIndex = WildWestGameInstance->GetAliveControllerIndex();
		AliveControllerIndex.Remove(CurrentSheriffIndex);

		UWorld* World = GetWorld();
		if (World)
		{
			ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
			if (DuelGameState)
			{
				DuelGameState->SetbIsDuelOver(AliveControllerIndex.IsEmpty());
			}
		}
	}
}

void ADuelPlayerController::ServerSetControllerIndex_Implementation()
{
	UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
	if (WildWestGameInstance)
	{
		int32 Index = -1;
		TArray<int32>& RemovedControllerIndex = WildWestGameInstance->GetRemovedControllerIndex();
		int32 CurrentSheriffIndex = WildWestGameInstance->GetCurrentSheriffIndex();

		for (int32 Idx = 0; Idx < RemovedControllerIndex.Num(); Idx++)
		{
			if (RemovedControllerIndex[Idx] > CurrentSheriffIndex)
			{
				Index = Idx;
				break;
			}
		}

		if (Index != -1)
		{
			RemovedControllerIndex.Insert(CurrentSheriffIndex, Index);
		}
		else
		{
			RemovedControllerIndex.AddUnique(CurrentSheriffIndex);
		}

		TArray<int32>& AliveControllerIndex = WildWestGameInstance->GetAliveControllerIndex();
		AliveControllerIndex.Remove(CurrentSheriffIndex);
		
		UWorld* World = GetWorld();
		if (World)
		{
			ADuelGameState* DuelGameState = World->GetGameState<ADuelGameState>();
			if (DuelGameState)
			{
				DuelGameState->SetbIsDuelOver(AliveControllerIndex.IsEmpty());
			}
		}
	}
}
