// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Gunman.generated.h"

UCLASS()
class WILDWEST_API AGunman : public ACharacter
{
	GENERATED_BODY()

public:
	AGunman();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void OpenVault();
	void RemoveVaultGauge();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(Replicated)
	class AVault* OverlappingVault;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> VaultGaugeClass;
	
	UPROPERTY()
	class UVaultGauge* VaultGauge;

	UFUNCTION(Server, Reliable)
	void ServerOpenVault();

	bool bIsInteracting;

public:
	FORCEINLINE void SetOverlappingVault(AVault* Vault) { OverlappingVault = Vault; }
};
