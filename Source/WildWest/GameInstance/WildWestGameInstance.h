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
	void ReplaceVaultList(FVector Key, bool Value);

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

	UPROPERTY(BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	TMap<FVector, bool> VaultList;

	UPROPERTY(BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> VaultTransformList;

	int32 CurrentSheriffIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	int32 VaultOpened;

	float MasterVolume{ 1.0f };

public:
	FORCEINLINE ECharacterState GetServerCharacterState() { return ServerCharacterState; }
	FORCEINLINE ECharacterState GetClientCharacterState() { return ClientCharacterState; }

	FORCEINLINE void SetServerCharacterState(ECharacterState NewState) { ServerCharacterState = NewState; }
	FORCEINLINE void SetClientCharacterState(ECharacterState NewState) { ClientCharacterState = NewState; }

	FORCEINLINE void SetbIsGameStarted(bool bIsStarted) { bIsGameStarted = bIsStarted; }

	FORCEINLINE TArray<FTransform>& GetLastTransformList() { return LastTransformList; }

	FORCEINLINE void SetLastGunmanTransform(FTransform LastTransform) { LastGunmanTransform = LastTransform; }

	FORCEINLINE TArray<int32>& GetAliveControllerIndex() { return AliveControllerIndex; }

	FORCEINLINE TArray<int32>& GetRemovedControllerIndex() { return RemovedControllerIndex; }

	FORCEINLINE TMap<FVector, bool>& GetVaultList() { return VaultList; }

	FORCEINLINE TArray<FTransform>& GetVaultTransformList() { return VaultTransformList; }

	FORCEINLINE int32 GetCurrentSheriffIndex() { return CurrentSheriffIndex; }
	FORCEINLINE void SetCurrentSheriffIndex(int32 NewIndex) { CurrentSheriffIndex = NewIndex; }

	FORCEINLINE void AddVaultOpened(int32 NewOpenedVault) { VaultOpened += NewOpenedVault; }
	FORCEINLINE void SetVaultOpened(int32 NewOpenedVault) { VaultOpened = NewOpenedVault; }

	FORCEINLINE float GetMasterVolume() { return MasterVolume; }
	FORCEINLINE void SetMasterVolume(float Volume) { MasterVolume = Volume; }
};
