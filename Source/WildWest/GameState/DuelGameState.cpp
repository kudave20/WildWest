// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameState.h"
#include "WildWest/Controller/DuelPlayerController.h"
#include "Kismet/GameplayStatics.h"

void ADuelGameState::StartDuel()
{
	if (GunmanDuelState == SheriffDuelState)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ADuelPlayerController* ServerPlayerController = Cast<ADuelPlayerController>(World->GetFirstPlayerController());
			ADuelPlayerController* ClientPlayerController = Cast<ADuelPlayerController>(UGameplayStatics::GetPlayerController(World, 1));
			if (ServerPlayerController && ClientPlayerController)
			{
				ServerPlayerController->MulticastBroadcast();
				ClientPlayerController->MulticastBroadcast();
			}
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