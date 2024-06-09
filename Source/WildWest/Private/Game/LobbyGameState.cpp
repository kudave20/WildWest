// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LobbyGameState.h"
#include "Player/LobbyPlayerController.h"
#include "Net/UnrealNetwork.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, bIsLobbyFull);
	DOREPLIFETIME(ALobbyGameState, ServerCharacterState);
	DOREPLIFETIME(ALobbyGameState, ClientCharacterState);
}

void ALobbyGameState::SetLobbyFull(bool bFull)
{
	bIsLobbyFull = bFull;
	
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LobbyPlayerController)
	{
		LobbyPlayerController->AddCharacterSelect();
	}
}

void ALobbyGameState::OnRep_bIsLobbyFull()
{
	if (bIsLobbyFull)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
			if (LobbyPlayerController)
			{
				LobbyPlayerController->AddCharacterSelect();
			}
		}
	}
}

void ALobbyGameState::SetServerCharacterState(ECharacterState NewState)
{
	if (NewState == ECharacterState::ECS_None) return;
	
	ServerCharacterState = NewState;
	
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LobbyPlayerController)
	{
		LobbyPlayerController->ChangeButtonState(NewState);
	}
}

void ALobbyGameState::OnRep_ServerCharacterState()
{
	if (ServerCharacterState == ECharacterState::ECS_None) return;
	
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LobbyPlayerController)
	{
		LobbyPlayerController->ChangeButtonState(ServerCharacterState);
	}
}

void ALobbyGameState::SetClientCharacterState(ECharacterState NewState)
{
	if (NewState == ECharacterState::ECS_None) return;
	
	ClientCharacterState = NewState;
	
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LobbyPlayerController)
	{
		LobbyPlayerController->ChangeButtonState(NewState);
	}
}

void ALobbyGameState::OnRep_ClientCharacterState()
{
	if (ClientCharacterState == ECharacterState::ECS_None) return;
	
	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (LobbyPlayerController)
	{
		LobbyPlayerController->ChangeButtonState(ClientCharacterState);
	}
}
