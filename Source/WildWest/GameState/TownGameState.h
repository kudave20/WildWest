// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "WildWest/Character/Sheriff.h"
#include "TownGameState.generated.h"


/**
 * 
 */
UCLASS()
class WILDWEST_API ATownGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "Gunman", meta = (AllowPrivateAccess = "true"))
	class AGunman* Gunman;

	UPROPERTY(Replicated)
	TArray<ASheriff*> SheriffList;

public:
	FORCEINLINE AGunman* GetGunman() const { return Gunman; }
	FORCEINLINE TArray<ASheriff*>& GetSheriffList() { return SheriffList; }
	FORCEINLINE void SetSheriffList(TArray<ASheriff*> InSheriffList) { SheriffList = InSheriffList; }

};
