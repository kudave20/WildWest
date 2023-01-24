// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Sheriff.generated.h"

UCLASS()
class WILDWEST_API ASheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ASheriff();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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
	void EnterDuel();
	
	UFUNCTION(Server, Reliable)
	void ServerSwitchToFirst();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToSecond();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToThird();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToFourth();

	UFUNCTION(Server, Reliable)
	void ServerEnterDuel();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	FRotator ControllerDirection;

	AController* CurrentController;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	int32 CharacterIndex;

public:
	FORCEINLINE FRotator GetControllerDirection() { return ControllerDirection; }
	FORCEINLINE void SetControllerDirection(FRotator NewControllerDirection) { ControllerDirection = NewControllerDirection; }

	FORCEINLINE AController* GetCurrentController() { return CurrentController; }
	FORCEINLINE void SetCurrentController(AController* NewCurrentController) { CurrentController = NewCurrentController; }

	FORCEINLINE int32 GetCharacterIndex() { return CharacterIndex; }
};
