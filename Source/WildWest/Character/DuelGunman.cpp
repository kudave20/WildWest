// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelGunman.h"
#include "Camera/CameraComponent.h"

ADuelGunman::ADuelGunman()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
}

void ADuelGunman::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADuelGunman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADuelGunman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

