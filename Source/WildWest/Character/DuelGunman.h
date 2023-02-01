// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuelGunman.generated.h"

UCLASS()
class WILDWEST_API ADuelGunman : public ACharacter
{
	GENERATED_BODY()

public:
	ADuelGunman();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ShootLeft();
	void ShootMiddle();
	void ShootRight();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayShootMontage(float PlayRate);

	UFUNCTION(BlueprintCallable)
	void SetIsDead(bool bIsDead);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerShootLeft();

	UFUNCTION(Server, Reliable)
	void ServerShootMiddle();

	UFUNCTION(Server, Reliable)
	void ServerShootRight();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<AActor> RevolverClass;

	UPROPERTY()
	AActor* Revolver;

public:

};
