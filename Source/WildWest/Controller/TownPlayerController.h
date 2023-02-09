// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TownPlayerController.generated.h"

UENUM(BlueprintType)
enum class EScreenIndex : uint8
{
	ECI_First UMETA(DisplayName = "First Screen"),
	ECI_Second UMETA(DisplayName = "Second Screen"),
	ECI_Third UMETA(DisplayName = "Third Screen"),
	ECI_Fourth UMETA(DisplayName = "Fourth Screen"),

	ECI_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetInitialControlRotation(const FRotator& NewRotation);

	UFUNCTION(Client, Reliable)
	void ClientRemovePlayer();

protected:
	virtual void PostSeamlessTravel() override;

private:
	EScreenIndex CurrentScreenIndex;

	UFUNCTION(Client, Reliable)
	void ClientSetInputModeGameOnly();

public:
	FORCEINLINE EScreenIndex GetCurrentScreenIndex() { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }

};
