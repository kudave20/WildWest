// Fill out your copyright notice in the Description page of Project Settings.


#include "WildWestGameInstance.h"

void UWildWestGameInstance::SetupServer(ECharacterState NewServerState)
{
	ServerCharacterState = NewServerState;
}

void UWildWestGameInstance::SetupClient(ECharacterState NewClientState)
{
	ClientCharacterState = NewClientState;
}

void UWildWestGameInstance::AddLastTransformList(FTransform LastTransform)
{
	LastTransformList.Add(LastTransform);
}

void UWildWestGameInstance::ReplaceVaultList(FVector Key, bool Value)
{
	VaultList[Key] = Value;
}
