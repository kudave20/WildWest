// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TownGameState.generated.h"

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
class WILDWEST_API ATownGameState : public AGameState
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, Category = "Sheriff", meta = (AllowPrivateAccess = "true"))
	TArray<class ASheriff*> SheriffList;

	EScreenIndex CurrentScreenIndex;

public:
	FORCEINLINE TArray<ASheriff*>& GetSheriffList() { return SheriffList; }

	FORCEINLINE EScreenIndex GetCurrentScreenIndex() { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }
};
