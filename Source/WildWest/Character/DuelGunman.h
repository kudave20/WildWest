// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuelGunman.generated.h"

UCLASS()
class WILDWEST_API ADuelGunman : public ACharacter
{
	GENERATED_BODY()

public:
	ADuelGunman();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void ShootLeft();
	void ShootMiddle();
	void ShootRight();

	UFUNCTION(Server, Reliable)
	void ServerShootLeft();

	UFUNCTION(Server, Reliable)
	void ServerShootMiddle();

	UFUNCTION(Server, Reliable)
	void ServerShootRight();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

public:

};
