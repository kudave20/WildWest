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
	UPROPERTY(VisibleAnywhere, Category = "Vault Properties")
	TObjectPtr<UStaticMeshComponent> VaultMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Vault Properties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> VaultDoorMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float MaxOpenTimer;

	float OpenTimer;

	UPROPERTY(BlueprintReadWrite, Category = "Flag", meta = (AllowPrivateAccess = "true"))
	bool bIsOpened;

public:	
	FORCEINLINE float GetMaxOpenTimer() const { return MaxOpenTimer; }
	FORCEINLINE float GetOpenTimer() const { return OpenTimer; }
	FORCEINLINE void SetOpenTimer(float NewTimer) { OpenTimer = NewTimer; }
	FORCEINLINE bool GetbIsOpened() const { return bIsOpened; }
	FORCEINLINE void SetbIsOpened(bool bOpened) { bIsOpened = bOpened; }
};
