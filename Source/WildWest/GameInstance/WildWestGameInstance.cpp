// Fill out your copyright notice in the Description page of Project Settings.


#include "WildWestGameInstance.h"

void UWildWestGameInstance::SetupServer(ECharacterState NewServerState)
{
	ServerCharacterState = NewServerState;
}

void UWildWestGameInstance::SetupClient(ECharacterState NewClientState)
{
	ClientCharacterState = NewClientState;

	switch (NewClientState)
	{
	case ECharacterState::ECS_Gunman:
		UE_LOG(LogTemp, Warning, TEXT("Client Gunman Setup!"));
		break;
	case ECharacterState::ECS_Sheriff:
		UE_LOG(LogTemp, Warning, TEXT("Client Sheriff Setup!"));
		break;
	}
}
