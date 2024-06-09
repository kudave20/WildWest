// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuelSheriff.generated.h"

UCLASS()
class WILDWEST_API ADuelSheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ADuelSheriff();
	virtual void Restart() override;

	void DodgeLeft();
	void Reload();
	void DodgeRight();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDodgeLeftMontage(float PlayRate);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDodgeRightMontage(float PlayRate);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetShoot(bool bShoot);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetIsSlow(bool bIsSlow);

	void SetIsDead(bool bIsDead);
	void SetShoot(bool bShoot);
	void SetIsSlow(bool bIsSlow);

protected:
	UFUNCTION(Server, Reliable)
	void ServerDodgeLeft();
	UFUNCTION(Server, Reliable)
	void ServerReload();
	UFUNCTION(Server, Reliable)
	void ServerDodgeRight();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<AActor> RevolverClass;

	UPROPERTY()
	AActor* Revolver;

	void SpawnRevolver();

public:

};
