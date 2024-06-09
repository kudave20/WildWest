// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DuelGunmanAnimInstance.h"

void UDuelGunmanAnimInstance::PlayShootMontage(float PlayRate)
{
	if (!Montage_IsPlaying(ShootMontage))
	{
		Montage_Play(ShootMontage, PlayRate);
	}
}