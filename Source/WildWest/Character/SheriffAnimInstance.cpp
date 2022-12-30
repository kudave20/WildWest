// Fill out your copyright notice in the Description page of Project Settings.


#include "SheriffAnimInstance.h"
#include "Sheriff.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USheriffAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Sheriff = Cast<ASheriff>(TryGetPawnOwner());
}

void USheriffAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Sheriff == nullptr)
	{
		Sheriff = Cast<ASheriff>(TryGetPawnOwner());
	}
	if (Sheriff == nullptr) return;

	FTransform T = Sheriff->GetActorTransform();
	FVector Velocity = Sheriff->GetVelocity();
	FRotator Rotation = UKismetMathLibrary::InverseTransformDirection(T, Velocity).Rotation();

	Angle = Rotation.Yaw;
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = Sheriff->GetCharacterMovement()->IsFalling();

	bIsAccelerating = Sheriff->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
}