// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DuelGunmanAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UDuelGunmanAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void PlayShootMontage(float PlayRate);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Shoot")
	UAnimMontage* ShootMontage;

	UPROPERTY(BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

public:
	FORCEINLINE void SetbIsDead(bool bDead) { bIsDead = bDead; }

};
