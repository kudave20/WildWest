// Fill out your copyright notice in the Description page of Project Settings.


#include "Vault.h"
#include "Components/SphereComponent.h"
#include "WildWest/Character/Gunman.h"
#include "WildWest/GameInstance/WildWestGameInstance.h"

AVault::AVault()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	VaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaultMesh"));
	SetRootComponent(VaultMesh);

	VaultDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaultDoorMesh"));
	VaultDoorMesh->SetupAttachment(RootComponent);
	VaultDoorMesh->SetIsReplicated(true);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AVault::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AVault::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AVault::OnSphereEndOverlap);
	}
}

void AVault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVault::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGunman* Gunman = Cast<AGunman>(OtherActor);
	if (Gunman)
	{
		Gunman->SetOverlappingVault(this);
	}
}

void AVault::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AGunman* Gunman = Cast<AGunman>(OtherActor);
	if (Gunman)
	{
		Gunman->SetOverlappingVault(nullptr);
	}
}
