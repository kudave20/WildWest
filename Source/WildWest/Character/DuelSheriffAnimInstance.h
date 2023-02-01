// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DuelSheriffAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UDuelSheriffAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void PlayDodgeLeftMontage(float PlayRate);
	void PlayDodgeRightMontage(float PlayRate);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeRightMontage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly, Category = "Shoot", meta = (AllowPrivateAccess = "true"))
	bool bShoot;

	UPROPERTY(BlueprintReadOnly, Category = "Shoot", meta = (AllowPrivateAccess = "true"))
	bool bIsSlow;

public:
	FORCEINLINE void SetbIsDead(bool bDead) { bIsDead = bDead; }
	FORCEINLINE void SetbShoot(bool bIsReady) { bShoot = bIsReady; }
	FORCEINLINE void SetbIsSlow(bool bSlow) { bIsSlow = bSlow; }

};
