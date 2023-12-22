// Fill out your copyright notice in the Description page of Project Settings.


#include "Vault.h"

AVault::AVault()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	VaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaultMesh"));
	SetRootComponent(VaultMesh);

	VaultDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaultDoorMesh"));
	VaultDoorMesh->SetupAttachment(RootComponent);
	VaultDoorMesh->SetIsReplicated(true);
}
