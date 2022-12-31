// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "WildWest/GameMode/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "WildWest/Character/GunmanController.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bIsLobbyFull);
}

void ALobbyGameState::OnRep_bIsLobbyFull()
{
	if (bIsLobbyFull)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AGunmanController* GunmanController = Cast<AGunmanController>(World->GetFirstPlayerController());
			if (GunmanController)
			{
				GunmanController->CreateCharacterSelect();
			}
		}
	}
}

void ALobbyGameState::SetbIsLobbyFull(bool bIsFull)
{
	bIsLobbyFull = bIsFull;

	UWorld* World = GetWorld();
	if (World)
	{
		APawn* Pawn = UGameplayStatics::GetPlayerPawn(World, 0);
		if (Pawn)
		{
			if (bIsLobbyFull && Pawn->IsLocallyControlled())
			{
				AGunmanController* GunmanController = Cast<AGunmanController>(Pawn->GetController());
				if (GunmanController)
				{
					GunmanController->CreateCharacterSelect();
				}
			}
		}
	}
}

void ALobbyGameState::SetbIsGunmanSelected(bool bIsSelected)
{
	bIsGunmanSelected = bIsSelected;

	if (bIsGunmanSelected && bIsSheriffSelected)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ALobbyGameMode* LobbyGameMode = World->GetAuthGameMode<ALobbyGameMode>();
			LobbyGameMode->TravelToTown();
		}
	}
}

void ALobbyGameState::SetbIsSheriffSelected(bool bIsSelected)
{
	bIsSheriffSelected = bIsSelected;

	if (bIsSheriffSelected && bIsGunmanSelected)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ALobbyGameMode* LobbyGameMode = World->GetAuthGameMode<ALobbyGameMode>();
			LobbyGameMode->TravelToTown();
		}
	}
}
