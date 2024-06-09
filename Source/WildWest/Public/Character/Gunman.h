// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gunman.generated.h"

class ATownPlayerController;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AVault;

UCLASS()
class WILDWEST_API AGunman : public ACharacter
{
	GENERATED_BODY()

public:
	AGunman();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> GunmanContext;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> InteractAction;

	void TraceVault(FHitResult& TraceResult);

	UPROPERTY()
	TObjectPtr<AActor> PreviousDetectedActor;
	
	UPROPERTY()
	TObjectPtr<AActor> DetectedActor;
	
	void UpdatePressWidget();
	void UpdateVaultGauge();

	UPROPERTY()
	TObjectPtr<ATownPlayerController> TownPlayerController;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float ArmLength = 150.f;

	UPROPERTY()
	TObjectPtr<AVault> TargetVault;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> PressClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> VaultGaugeClass;
	
	void InteractWithVault();
	void StopOpeningVault();
	
	UFUNCTION(Server, Reliable)
	void ServerOpenVault();

	bool bIsInteracting;
};
