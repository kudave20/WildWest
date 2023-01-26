// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameState.h"
#include "WildWest/Character/DuelGunman.h"
#include "WildWest/Character/DuelSheriff.h"
#include "WildWest/Controller/DuelPlayerController.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ADuelGameState::StartDuel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ADuelPlayerController* ServerPlayerController = Cast<ADuelPlayerController>(World->GetFirstPlayerController());
		ADuelPlayerController* ClientPlayerController = Cast<ADuelPlayerController>(UGameplayStatics::GetPlayerController(World, 1));

		UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
		if (WildWestGameInstance)
		{
			ADuelGunman* DuelGunman = nullptr;
			ADuelSheriff* DuelSheriff = nullptr;

			switch (WildWestGameInstance->GetServerCharacterState())
			{
			case ECharacterState::ECS_Gunman:
				DuelGunman = Cast<ADuelGunman>(ServerPlayerController->GetPawn());
				if (DuelGunman)
				{
					DuelGunman->PlayShootMontage();
				}

				break;
			case ECharacterState::ECS_Sheriff:
				DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
				if (DuelSheriff)
				{
					if (SheriffDuelState == EDuelState::EDS_Left)
					{
						DuelSheriff->PlayDodgeLeftMontage();
					}
					else if (SheriffDuelState == EDuelState::EDS_Right)
					{
						DuelSheriff->PlayDodgeRightMontage();
					}
				}

				break;
			}

			switch (WildWestGameInstance->GetClientCharacterState())
			{
			case ECharacterState::ECS_Gunman:
				DuelGunman = Cast<ADuelGunman>(ClientPlayerController->GetPawn());
				if (DuelGunman)
				{
					DuelGunman->PlayShootMontage();
				}

				break;
			case ECharacterState::ECS_Sheriff:
				DuelSheriff = Cast<ADuelSheriff>(ClientPlayerController->GetPawn());
				if (DuelSheriff)
				{
					if (SheriffDuelState == EDuelState::EDS_Left)
					{
						DuelSheriff->PlayDodgeLeftMontage();
					}
					else if (SheriffDuelState == EDuelState::EDS_Right)
					{
						DuelSheriff->PlayDodgeRightMontage();
					}
				}

				break;
			}
		}

		if (GunmanDuelState == SheriffDuelState)
		{
			if (ServerPlayerController && ClientPlayerController)
			{
				ServerPlayerController->MulticastBroadcast();
				ClientPlayerController->MulticastBroadcast();
			}
		}
	}
}