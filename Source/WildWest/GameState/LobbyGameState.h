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
	void GunmanButtonClicked();
	void SheriffButtonClicked();

	void ServerGunmanButtonClicked();
	void ServerSheriffButtonClicked();

	void WidgetSetup();

	void SetServerPlayerController(APlayerController* NewPlayer);
	void SetClientPlayerController(APlayerController* NewPlayer);

private:
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<AActor> GunmanClass;
	UPROPERTY(EditDefaultsOnly, Category = Character)
	TSubclassOf<AActor> SheriffClass;

	class ALobbyPlayerController* ServerPlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_ClientPlayerController)
	ALobbyPlayerController* ClientPlayerController;
	
	UFUNCTION()
	void OnRep_ClientPlayerController();

	void CheckSelectedCharacter();

	void SetupServer(ECharacterState NewServerState);
	void SetupClient(ECharacterState NewClientState);

	bool bIsGunmanSelected;
	bool bIsSheriffSelected;

public:
	FORCEINLINE ALobbyPlayerController* GetServerPlayerController() { return ServerPlayerController; }
	FORCEINLINE ALobbyPlayerController* GetClientPlayerController() { return ClientPlayerController; }
	
};
