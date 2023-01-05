// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildWest/GameState/TownGameState.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Sheriff.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectFirstScreenComplete, EScreenIndex, LastScreenIndex, EScreenIndex, CurrentScreenIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectSecondScreenComplete, EScreenIndex, LastScreenIndex, EScreenIndex, CurrentScreenIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectThirdScreenComplete, EScreenIndex, LastScreenIndex, EScreenIndex, CurrentScreenIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectFourthScreenComplete, EScreenIndex, LastScreenIndex, EScreenIndex, CurrentScreenIndex);

UCLASS()
class WILDWEST_API ASheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ASheriff();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void SetScreenVisibility(bool bNewVisibility);

	UPROPERTY(BlueprintAssignable)
	FOnSelectFirstScreenComplete SelectFirstScreenCompleteDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnSelectSecondScreenComplete SelectSecondScreenCompleteDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnSelectThirdScreenComplete SelectThirdScreenCompleteDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnSelectFourthScreenComplete SelectFourthScreenCompleteDelegate;

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
	void SetbIsRotatingFalse();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Screen")
	class USceneCaptureComponent2D* Screen;

	UPROPERTY(Replicated)
	FRotator ControllerDirection;

	UFUNCTION(Server, Reliable)
	void ServerSwitchToFirst();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToSecond();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToThird();

	UFUNCTION(Server, Reliable)
	void ServerSwitchToFourth();

	void TrembleCharacter();

	AController* CurrentController;

	bool bToggle{ false };

public:
	FORCEINLINE FRotator GetControllerDirection() { return ControllerDirection; }

	FORCEINLINE AController* GetCurrentController() { return CurrentController; }
	FORCEINLINE void SetCurrentController(AController* NewController) { CurrentController = NewController; }
};
