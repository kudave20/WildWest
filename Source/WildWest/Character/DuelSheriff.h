// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuelSheriff.generated.h"

UCLASS()
class WILDWEST_API ADuelSheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ADuelSheriff();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DodgeLeft();
	void Reload();
	void DodgeRight();

	void PlayDodgeLeftMontage();
	void PlayDodgeRightMontage();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerDodgeLeft();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	UFUNCTION(Server, Reliable)
	void ServerDodgeRight();

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

public:

};
