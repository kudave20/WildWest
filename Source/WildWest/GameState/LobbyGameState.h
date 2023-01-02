// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Gunman UMETA(DisplayName = "Gunman"),
	ECS_Sheriff UMETA(DisplayName = "Sheriff"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void GunmanButtonClicked();
	void SheriffButtonClicked();

	void ServerGunmanButtonClicked();
	void ServerSheriffButtonClicked();

private:
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<AActor> GunmanClass;
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<AActor> SheriffClass;

	UPROPERTY(ReplicatedUsing = OnRep_bIsLobbyFull)
	bool bIsLobbyFull{ false };

	UFUNCTION()
	void OnRep_bIsLobbyFull();

	void CheckSelectedCharacter();

	void SetupSpawnCharacter(ECharacterState CharacterState, FTransform Transform);

	bool bIsGunmanSelected{ false };
	bool bIsSheriffSelected{ false };

public:
	void SetbIsLobbyFull(bool bIsFull);
	
};
