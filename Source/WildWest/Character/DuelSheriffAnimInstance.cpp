// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriffAnimInstance.h"

void UDuelSheriffAnimInstance::PlayDodgeLeftMontage()
{
	if (!Montage_IsPlaying(DodgeLeftMontage))
	{
		Montage_Play(DodgeLeftMontage, 1.0f);
	}
}

void UDuelSheriffAnimInstance::PlayDodgeRightMontage()
{
	if (!Montage_IsPlaying(DodgeRightMontage))
	{
		Montage_Play(DodgeRightMontage, 1.0f);
	}
}