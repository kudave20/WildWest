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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSheriffListReplicateComplete)

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SheriffListSetup();

	UPROPERTY(BlueprintAssignable)
	FOnSheriffListReplicateComplete SheriffListReplicateCompleteDelegate;
	
private:
	UPROPERTY(ReplicatedUsing = OnRep_SheriffList, BlueprintReadOnly, Category = "Sheriff", meta = (AllowPrivateAccess = "true"))
	TArray<class ASheriff*> SheriffList;

	UFUNCTION()
	void OnRep_SheriffList();

	UPROPERTY(Replicated)
	EScreenIndex LastScreenIndex;
	UPROPERTY(ReplicatedUsing = OnRep_CurrentScreenIndex)
	EScreenIndex CurrentScreenIndex;

	UFUNCTION()
	void OnRep_CurrentScreenIndex();

public:
	FORCEINLINE TArray<ASheriff*>& GetSheriffList() { return SheriffList; }
	FORCEINLINE void SetSheriffList(TArray<ASheriff*> List) { SheriffList = List; }

	FORCEINLINE EScreenIndex GetLastScreenIndex() { return LastScreenIndex; }
	FORCEINLINE void SetLastScreenIndex(EScreenIndex NewScreenIndex) { LastScreenIndex = NewScreenIndex; }

	FORCEINLINE EScreenIndex GetCurrentScreenIndex() { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }
};
