// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGameState.h"
#include "WildWest/Character/DuelGunman.h"
#include "WildWest/Character/DuelSheriff.h"
#include "WildWest/Controller/DuelPlayerController.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ADuelGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADuelGameState, SheriffDuelState);
	DOREPLIFETIME(ADuelGameState, DuelSheriff);
	DOREPLIFETIME(ADuelGameState, DuelTimer);
}

void ADuelGameState::StartDuel()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);

		ADuelPlayerController* ServerPlayerController = Cast<ADuelPlayerController>(World->GetFirstPlayerController());
		ADuelPlayerController* ClientPlayerController = Cast<ADuelPlayerController>(UGameplayStatics::GetPlayerController(World, 1));

		if (ServerPlayerController && ClientPlayerController)
		{
			UWildWestGameInstance* WildWestGameInstance = GetGameInstance<UWildWestGameInstance>();
			if (WildWestGameInstance)
			{
				if (GunmanDuelState == SheriffDuelState)
				{
					if (WildWestGameInstance->GetServerCharacterState() == ECharacterState::ECS_Gunman)
					{
						DuelGunman = Cast<ADuelGunman>(ServerPlayerController->GetPawn());
						DuelSheriff = Cast<ADuelSheriff>(ClientPlayerController->GetPawn());
						if (DuelGunman)
						{
							DuelGunman->MulticastPlayShootMontage(0.5f);

							if (SheriffDuelState == EDuelState::EDS_Left)
							{
								FTimerHandle WaitHandle;
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
										}
									}), 2.0f, false);
							}
							else if (SheriffDuelState == EDuelState::EDS_Right)
							{
								FTimerHandle WaitHandle;
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
										}
									}), 2.0f, false);
							}
						}

						ServerPlayerController->FireDelegate.Broadcast();
						ClientPlayerController->ClientHitBroadcast();
					}
					else if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Gunman)
					{
						DuelGunman = Cast<ADuelGunman>(ClientPlayerController->GetPawn());
						DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
						if (DuelGunman)
						{
							DuelGunman->MulticastPlayShootMontage(0.5f);

							if (SheriffDuelState == EDuelState::EDS_Left)
							{
								FTimerHandle WaitHandle;
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
										}
									}), 2.0f, false);
							}
							else if (SheriffDuelState == EDuelState::EDS_Right)
							{
								FTimerHandle WaitHandle;
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
										}
									}), 2.0f, false);
							}
						}

						ServerPlayerController->FireDelegate.Broadcast();
						ClientPlayerController->ClientFireBroadcast();
					}

					if (ServerPlayerController && ClientPlayerController)
					{
						ServerPlayerController->HitDelegate.Broadcast();
						ClientPlayerController->ClientHitBroadcast();
					}
				}
				else
				{
					switch (WildWestGameInstance->GetServerCharacterState())
					{
					case ECharacterState::ECS_Gunman:
						DuelGunman = Cast<ADuelGunman>(ServerPlayerController->GetPawn());
						DuelSheriff = Cast<ADuelSheriff>(ClientPlayerController->GetPawn());
						if (DuelGunman)
						{
							DuelGunman->MulticastPlayShootMontage(1.0f);
						}

						break;
					case ECharacterState::ECS_Sheriff:
						DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
						if (DuelSheriff)
						{
							if (SheriffDuelState == EDuelState::EDS_Left)
							{
								DuelSheriff->MulticastPlayDodgeLeftMontage(1.0f);
							}
							else if (SheriffDuelState == EDuelState::EDS_Right)
							{
								DuelSheriff->MulticastPlayDodgeRightMontage(1.0f);
							}
						}

						break;
					}

					switch (WildWestGameInstance->GetClientCharacterState())
					{
					case ECharacterState::ECS_Gunman:
						DuelGunman = Cast<ADuelGunman>(ClientPlayerController->GetPawn());
						DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
						if (DuelGunman)
						{
							DuelGunman->MulticastPlayShootMontage(1.0f);
						}

						break;
					case ECharacterState::ECS_Sheriff:
						DuelSheriff = Cast<ADuelSheriff>(ClientPlayerController->GetPawn());
						if (DuelSheriff)
						{
							if (SheriffDuelState == EDuelState::EDS_Left)
							{
								DuelSheriff->MulticastPlayDodgeLeftMontage(1.0f);
							}
							else if (SheriffDuelState == EDuelState::EDS_Right)
							{
								DuelSheriff->MulticastPlayDodgeRightMontage(1.0f);
							}
						}

						break;
					}

					ServerPlayerController->DodgeDelegate.Broadcast();
					ClientPlayerController->ClientDodgeBroadcast();
				}

				ServerPlayerController->DuelSelectCompleteDelegate.Broadcast();
				ClientPlayerController->ClientDuelSelectCompleteBroadcast();;
			}
		}
	}
}

void ADuelGameState::StartDuelTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				DuelTimer--;
				if (DuelTimer == 0)
				{
					if (GunmanDuelState == EDuelState::EDS_Initial)
					{
						GunmanDuelState = EDuelState::EDS_Middle;
					}
					if (SheriffDuelState == EDuelState::EDS_Initial)
					{
						SheriffDuelState = EDuelState::EDS_Middle;
					}

					StartDuel();
				}
			}), 1.0f, true);
	}
}
