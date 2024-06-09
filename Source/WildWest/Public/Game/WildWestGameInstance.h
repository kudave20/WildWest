// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WildWestTypes/CharacterState.h"
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

	void CheckSelectedCharacter();

	void ReplaceVaultList(FVector Key, bool Value);

	UPROPERTY()
	TArray<FTransform> LastTransformList;
	FTransform LastGunmanTransform;

	UPROPERTY()
	TArray<bool> SheriffDeathList;

	bool bLastChase = false;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ECharacterState ServerCharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	ECharacterState ClientCharacterState;
	
	bool bIsGameStarted;

	UPROPERTY(BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	TMap<FVector, bool> VaultList;

	UPROPERTY(BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> VaultTransformList;

	UPROPERTY(BlueprintReadOnly, Category = "Vault", meta = (AllowPrivateAccess = "true"))
	int32 VaultOpened;

	float MasterVolume{ 1.0f };

public:
	FORCEINLINE ECharacterState GetServerCharacterState() const { return ServerCharacterState; }
	FORCEINLINE ECharacterState GetClientCharacterState() const { return ClientCharacterState; }
	FORCEINLINE void SetServerCharacterState(ECharacterState NewState) { ServerCharacterState = NewState; }
	FORCEINLINE void SetClientCharacterState(ECharacterState NewState) { ClientCharacterState = NewState; }
	FORCEINLINE bool IsGameStarted() const { return bIsGameStarted; }
	FORCEINLINE void StartGame(bool bIsStarted) { bIsGameStarted = bIsStarted; }
	FORCEINLINE TMap<FVector, bool>& GetVaultList() { return VaultList; }
	FORCEINLINE TArray<FTransform>& GetVaultTransformList() { return VaultTransformList; }
	FORCEINLINE void AddVaultOpened(int32 NewOpenedVault) { VaultOpened += NewOpenedVault; }
	FORCEINLINE void SetVaultOpened(int32 NewOpenedVault) { VaultOpened = NewOpenedVault; }
	FORCEINLINE float GetMasterVolume() { return MasterVolume; }
	FORCEINLINE void SetMasterVolume(float Volume) { MasterVolume = Volume; }
};
