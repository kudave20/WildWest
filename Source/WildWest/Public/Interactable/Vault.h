// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vault.generated.h"

UCLASS()
class WILDWEST_API AVault : public AActor
{
	GENERATED_BODY()
	
public:	
	AVault();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenDoor();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> VaultMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> VaultDoorMesh;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float MaxOpenTimer;

	float OpenTimer;

	bool bIsOpened;

public:	
	FORCEINLINE float GetMaxOpenTimer() const { return MaxOpenTimer; }
	FORCEINLINE float GetOpenTimer() const { return OpenTimer; }
	FORCEINLINE void SetOpenTimer(float NewTimer) { OpenTimer = NewTimer; }
	FORCEINLINE bool IsOpened() const { return bIsOpened; }
	FORCEINLINE void Open(bool bOpened) { bIsOpened = bOpened; }
};
