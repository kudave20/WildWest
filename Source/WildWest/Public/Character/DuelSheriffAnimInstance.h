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
	TObjectPtr<UAnimMontage> DodgeLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	TObjectPtr<UAnimMontage> DodgeRightMontage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Death", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(BlueprintReadOnly, Category = "Shoot", meta = (AllowPrivateAccess = "true"))
	bool bShoot;

	UPROPERTY(BlueprintReadOnly, Category = "Shoot", meta = (AllowPrivateAccess = "true"))
	bool bIsSlow;

public:
	FORCEINLINE void SetIsDead(bool bDead) { bIsDead = bDead; }
	FORCEINLINE void SetShoot(bool bIsReady) { bShoot = bIsReady; }
	FORCEINLINE void SetIsSlow(bool bSlow) { bIsSlow = bSlow; }

};
