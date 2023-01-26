// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGunmanAnimInstance.h"

void UDuelGunmanAnimInstance::PlayShootMontage()
{
	if (!Montage_IsPlaying(ShootMontage))
	{
		Montage_Play(ShootMontage, 1.0f);
	}
}