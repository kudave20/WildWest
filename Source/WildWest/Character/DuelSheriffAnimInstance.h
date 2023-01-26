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
	void PlayDodgeLeftMontage();
	void PlayDodgeRightMontage();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	UAnimMontage* DodgeRightMontage;
};
