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
	DOREPLIFETIME(ADuelGameState, DuelGunman);
	DOREPLIFETIME(ADuelGameState, DuelSheriff);
	DOREPLIFETIME(ADuelGameState, DuelTimer);
	DOREPLIFETIME(ADuelGameState, BulletCount);
	DOREPLIFETIME(ADuelGameState, bIsDuelOver);
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
							DuelGunman->MulticastPlayShootMontage(0.25f);

							FTimerHandle WaitHandle;
							switch (SheriffDuelState)
							{
							case EDuelState::EDS_Left:
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
										}
									}), 2.0f, false);

								break;
							case EDuelState::EDS_Middle:
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											AController* SheriffController = DuelSheriff->GetController();
											if (SheriffController)
											{
												SheriffController->SetControlRotation(FRotator(0, 90, 0));
											}

											DuelSheriff->MulticastSetIsSlow(true);
											DuelSheriff->MulticastSetShoot(true);
										}
									}), 2.0f, false);

								break;
							case EDuelState::EDS_Right:
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
										}
									}), 2.0f, false);

								break;
							}
						}
					}
					else if (WildWestGameInstance->GetClientCharacterState() == ECharacterState::ECS_Gunman)
					{
						DuelGunman = Cast<ADuelGunman>(ClientPlayerController->GetPawn());
						DuelSheriff = Cast<ADuelSheriff>(ServerPlayerController->GetPawn());
						if (DuelGunman)
						{
							DuelGunman->MulticastPlayShootMontage(0.25f);

							FTimerHandle WaitHandle;
							switch (SheriffDuelState)
							{
							case EDuelState::EDS_Left:
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeLeftMontage(0.25f);
										}
									}), 2.0f, false);

								break;
							case EDuelState::EDS_Middle:
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											AController* SheriffController = DuelSheriff->GetController();
											if (SheriffController)
											{
												SheriffController->SetControlRotation(FRotator(0, 90, 0));
											}

											DuelSheriff->MulticastSetIsSlow(true);
											DuelSheriff->MulticastSetShoot(true);
										}
									}), 2.0f, false);

								break;
							case EDuelState::EDS_Right:
								World->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
									{
										if (DuelSheriff)
										{
											DuelSheriff->MulticastPlayDodgeRightMontage(0.25f);
										}
									}), 2.0f, false);

								break;
							}
						}
					}

					ServerPlayerController->FireDelegate.Broadcast();
					ClientPlayerController->ClientFireBroadcast();

					ServerPlayerController->HitDelegate.Broadcast();
					ClientPlayerController->ClientHitBroadcast();
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
							switch (SheriffDuelState)
							{
							case EDuelState::EDS_Left:
								DuelSheriff->MulticastPlayDodgeLeftMontage(1.0f);
								break;
							case EDuelState::EDS_Middle:
								ServerPlayerController->SetControlRotation(FRotator(0, 90, 0));
								ServerPlayerController->FightBackDelegate.Broadcast();
								ClientPlayerController->ClientFightBackBroadcast();

								bIsDuelOver = true;
								World->GetTimerManager().ClearTimer(TimerHandle);

								DuelSheriff->MulticastSetShoot(true);
								break;
							case EDuelState::EDS_Right:
								DuelSheriff->MulticastPlayDodgeRightMontage(1.0f);
								break;
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
							switch (SheriffDuelState)
							{
							case EDuelState::EDS_Left:
								DuelSheriff->MulticastPlayDodgeLeftMontage(1.0f);
								break;
							case EDuelState::EDS_Middle:
								ClientPlayerController->SetControlRotation(FRotator(0, 90, 0));
								ServerPlayerController->FightBackDelegate.Broadcast();
								ClientPlayerController->ClientFightBackBroadcast();

								bIsDuelOver = true;
								World->GetTimerManager().ClearTimer(TimerHandle);

								DuelSheriff->MulticastSetShoot(true);
								break;
							case EDuelState::EDS_Right:
								DuelSheriff->MulticastPlayDodgeRightMontage(1.0f);
								break;
							}
						}

						break;
					}

					ServerPlayerController->DodgeDelegate.Broadcast();
					ClientPlayerController->ClientDodgeBroadcast();

					BulletCount--;

					if (BulletCount == 0)
					{
						ServerPlayerController->FireCompleteDelegate.Broadcast();
						ClientPlayerController->ClientFireCompleteBroadcast();

						bIsDuelOver = true;
						World->GetTimerManager().ClearTimer(TimerHandle);
					}
				}

				ServerPlayerController->DuelSelectCompleteDelegate.Broadcast();
				ClientPlayerController->ClientDuelSelectCompleteBroadcast();

				FInputModeGameOnly InputModeData;
				ServerPlayerController->SetInputMode(InputModeData);
				ServerPlayerController->SetShowMouseCursor(false);

				ClientPlayerController->SetInputMode(InputModeData);
				ClientPlayerController->SetShowMouseCursor(false);
			}
		}
	}
}

void ADuelGameState::StartDuelTimer()
{
	UWorld* World = GetWorld();
	if (World)
	{
		DuelTimer = InitialDuelTimer;

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

void ADuelGameState::ResetDuelState()
{
	if (HasAuthority())
	{
		GunmanDuelState = EDuelState::EDS_Initial;
		SheriffDuelState = EDuelState::EDS_Initial;
	}
}
