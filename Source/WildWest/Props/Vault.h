// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vault.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenDoorDelegate);

UCLASS()
class WILDWEST_API AVault : public AActor
{
	GENERATED_BODY()
	
public:	
	AVault();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnOpenDoorDelegate OpenDoorDelegate;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	UPROPERTY(VisibleAnywhere, Category = "Vault Properties")
	UStaticMeshComponent* VaultMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Vault Properties", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* VaultDoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Vault Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float MaxOpenTimer;

	float OpenTimer;

public:	
	FORCEINLINE float GetMaxOpenTimer() { return MaxOpenTimer; }

	FORCEINLINE float GetOpenTimer() { return OpenTimer; }
	FORCEINLINE void SetOpenTimer(float NewTimer) { OpenTimer = NewTimer; }
};
