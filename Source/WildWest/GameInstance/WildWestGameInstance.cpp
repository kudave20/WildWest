// Fill out your copyright notice in the Description page of Project Settings.


#include "WildWestGameInstance.h"

void UWildWestGameInstance::SetupSpawn(ECharacterState NewState, FTransform Transform)
{
	CharacterState = NewState;
	SpawnTransform = Transform;
}