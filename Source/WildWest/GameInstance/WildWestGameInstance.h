// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWestGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UWildWestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetupServer(ECharacterState NewServerState);
	void SetupClient(ECharacterState NewClientState);

	void AddLastTransformList(FTransform LastTransform);

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ECharacterState ServerCharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ECharacterState ClientCharacterState;

	UPROPERTY(BlueprintReadWrite, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	bool bIsGameStarted;

	UPROPERTY(BlueprintReadOnly, Category = "Transform", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> LastTransformList;

	UPROPERTY(BlueprintReadOnly, Category = "Transform", meta = (AllowPrivateAccess = "true"))
	FTransform LastGunmanTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Index", meta = (AllowPrivateAccess = "true"))
	TArray<int32> AliveControllerIndex{ 1, 2, 3, 4 };

	UPROPERTY(BlueprintReadOnly, Category = "Index", meta = (AllowPrivateAccess = "true"))
	TArray<int32> RemovedControllerIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Index", meta = (AllowPrivateAccess = "true"))
	int32 CurrentSheriffIndex;

	float MasterVolume{ 1.0f };

public:
	FORCEINLINE ECharacterState GetServerCharacterState() { return ServerCharacterState; }
	FORCEINLINE ECharacterState GetClientCharacterState() { return ClientCharacterState; }

	FORCEINLINE void SetbIsGameStarted(bool bIsStarted) { bIsGameStarted = bIsStarted; }

	FORCEINLINE TArray<FTransform>& GetLastTransformList() { return LastTransformList; }

	FORCEINLINE void SetLastGunmanTransform(FTransform LastTransform) { LastGunmanTransform = LastTransform; }

	FORCEINLINE void SetCurrentSheriffIndex(int32 NewIndex) { CurrentSheriffIndex = NewIndex; }

	FORCEINLINE float GetMasterVolume() { return MasterVolume; }
	FORCEINLINE void SetMasterVolume(float Volume) { MasterVolume = Volume; }
};
