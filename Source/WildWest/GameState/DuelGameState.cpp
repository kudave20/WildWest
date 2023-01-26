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

			if (GunmanDuelState == SheriffDuelState)
			{
				switch (WildWestGameInstance->GetServerCharacterState())
				{
				case ECharacterState::ECS_Gunman:
					DuelGunman = Cast<ADuelGunman>(ServerPlayerController->GetPawn());
					if (DuelGunman)
					{
						DuelGunman->MulticastPlayShootMontage(0.5f);

						if (SheriffDuelState == EDuelState::EDS_Left)
						{
							FTimerHandle WaitHandle;
							World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
								{
									UWorld* World = GetWorld();
									if (World)
									{
										ADuelPlayerController* ClientPlayerController = Cast<ADuelPlayerController>(UGameplayStatics::GetPlayerController(World, 1));
										if (ClientPlayerController)
										{
											ADuelSheriff* DuelSheriff = Cast<ADuelSheriff>(ClientPlayerController->GetPawn());
											if (DuelSheriff)
											{
												DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
											}
										}
									}
								}), 2.0f, false);
						}
						else if (SheriffDuelState == EDuelState::EDS_Right)
						{
							FTimerHandle WaitHandle;
							World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
								{
									UWorld* World = GetWorld();
									if (World)
									{
										ADuelPlayerController* ClientPlayerController = Cast<ADuelPlayerController>(UGameplayStatics::GetPlayerController(World, 1));
										if (ClientPlayerController)
										{
											ADuelSheriff* DuelSheriff = Cast<ADuelSheriff>(ClientPlayerController->GetPawn());
											if (DuelSheriff)
											{
												DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
											}
										}
									}
								}), 2.0f, false);
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
						DuelGunman->MulticastPlayShootMontage(0.5f);

						if (SheriffDuelState == EDuelState::EDS_Left)
						{
							FTimerHandle WaitHandle;
							World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
								{
									UWorld* World = GetWorld();
									if (World)
									{
										ADuelPlayerController* ServerPlayerController = Cast<ADuelPlayerController>(World->GetFirstPlayerController());
										if (ServerPlayerController)
										{
											ADuelSheriff* DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
											if (DuelSheriff)
											{
												DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
											}
										}
									}
								}), 2.0f, false);
						}
						else if (SheriffDuelState == EDuelState::EDS_Right)
						{
							FTimerHandle WaitHandle;
							World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
								{
									UWorld* World = GetWorld();
									if (World)
									{
										ADuelPlayerController* ServerPlayerController = Cast<ADuelPlayerController>(World->GetFirstPlayerController());
										if (ServerPlayerController)
										{
											ADuelSheriff* DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
											if (DuelSheriff)
											{
												DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
											}
										}
									}
								}), 2.0f, false);
						}
					}

					break;
				}

				if (ServerPlayerController && ClientPlayerController)
				{
					ServerPlayerController->MulticastStartDuelBroadcast();
					ClientPlayerController->MulticastStartDuelBroadcast();
				}
			}
			else
			{
				switch (WildWestGameInstance->GetServerCharacterState())
				{
				case ECharacterState::ECS_Gunman:
					DuelGunman = Cast<ADuelGunman>(ServerPlayerController->GetPawn());
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

				ServerPlayerController->MulticastDodgeBroadcast();
				ClientPlayerController->MulticastDodgeBroadcast();
			}
		}
	}
}
