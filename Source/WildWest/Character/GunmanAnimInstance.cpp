// Fill out your copyright notice in the Description page of Project Settings.


#include "GunmanAnimInstance.h"
#include "Gunman.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGunmanAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Gunman = Cast<AGunman>(TryGetPawnOwner());
}

void UGunmanAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Gunman == nullptr)
	{
		Gunman = Cast<AGunman>(TryGetPawnOwner());
	}
	if (Gunman == nullptr) return;

	FVector Velocity = Gunman->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Gunman->GetCharacterMovement()->IsFalling();

	bIsAccelerating = Gunman->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}