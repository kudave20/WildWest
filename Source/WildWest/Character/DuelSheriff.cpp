// Fill out your copyright notice in the Description page of Project Settings.


#include "DuelSheriff.h"
#include "Camera/CameraComponent.h"

ADuelSheriff::ADuelSheriff()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetMesh());
	Camera->bUsePawnControlRotation = true;
}

void ADuelSheriff::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADuelSheriff::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADuelSheriff::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

