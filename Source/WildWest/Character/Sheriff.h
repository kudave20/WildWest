// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Sheriff.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	EPS_First UMETA(DisplayName = "First State"),
	EPS_Second UMETA(DisplayName = "Second State"),
	EPS_Third UMETA(DisplayName = "Third State"),
	EPS_Fourth UMETA(DisplayName = "Fourth State"),

	EPS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class WILDWEST_API ASheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ASheriff();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void SwitchToFirst();
	void SwitchToSecond();
	void SwitchToThird();
	void SwitchToFourth();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY()
	TArray<ASheriff*> SheriffList;
public:

};
