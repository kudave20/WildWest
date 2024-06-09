// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GunmanAnimInstance.h"
#include "Character/Gunman.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	FTransform T = Gunman->GetActorTransform();
	FVector Velocity = Gunman->GetVelocity();
	FRotator Rotation = UKismetMathLibrary::InverseTransformDirection(T, Velocity).Rotation();

	Angle = Rotation.Yaw;
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Gunman->GetCharacterMovement()->IsFalling();
}