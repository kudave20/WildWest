// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriffAnimInstance.h"

void UDuelSheriffAnimInstance::PlayDodgeLeftMontage(float PlayRate)
{
	if (!Montage_IsPlaying(DodgeLeftMontage))
	{
		Montage_Play(DodgeLeftMontage, PlayRate);
	}
}

void UDuelSheriffAnimInstance::PlayDodgeRightMontage(float PlayRate)
{
	if (!Montage_IsPlaying(DodgeRightMontage))
	{
		Montage_Play(DodgeRightMontage, PlayRate);
	}
}
