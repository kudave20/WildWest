// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildWestTypes/ScreenIndex.h"
#include "Sheriff.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ATownPlayerController;
class ATownGameState;
class UWildWestGameInstance;

UCLASS()
class WILDWEST_API ASheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ASheriff();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Restart() override;
	virtual void UnPossessed() override;

	void InitialSwitch(int32 CurrentSheriffNum);
	
	void Stun();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> SheriffContext;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> FirstSwitchAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> SecondSwitchAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> ThirdSwitchAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> FourthSwitchAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> InteractAction;
	
	void FirstSwitch();
	void SecondSwitch();
	void ThirdSwitch();
	void FourthSwitch();
	void Interact();
	void EnterDuel();
	
	UFUNCTION(Server, Reliable)
	void ServerSwitch(EScreenIndex ScreenIndex);
	UFUNCTION(Server, Reliable)
	void ServerInteract();

	bool CanSwitch(EScreenIndex ScreenIndex);

	bool bInputEnabled = true;

	UPROPERTY()
	TObjectPtr<ATownPlayerController> PossessingController;
	UPROPERTY()
	TObjectPtr<ATownPlayerController> FirstPlayerController;
	UPROPERTY()
	TObjectPtr<ATownGameState> TownGameState;
	UPROPERTY()
	TObjectPtr<UWildWestGameInstance> WildWestGameInstance;

	FRotator PreviousDirection;

	bool TraceGunman();
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	float ArmLength = 150.f;

	bool LineOfSightTo(AActor* Other) const;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float SightLength = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float ViewportThreshold = 2.f;
	
	void LookAt(AActor* Other);

	void UpdateControlTime(float DeltaTime);
	void UpdateViewport();
	void UpdateSheriffHUDViewport();
	void HideSheriffHUDViewport();
	void HideSheriffHUDViewport(bool bHideImmediately);

	UPROPERTY(Replicated)
	bool bUpdateViewport = false;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float InitialControlTime = 10.f;
	float ControlTime;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float StunTime;

	UPROPERTY(Replicated)
	int32 SheriffIndex;

	bool bWasDetectingPreviousFrame = false;

public:
	FORCEINLINE FRotator GetPreviousDirection() const { return PreviousDirection; }
	FORCEINLINE TObjectPtr<UCameraComponent> GetCamera() const { return Camera; }
	FORCEINLINE void SetSheriffIndex(int32 Index) { SheriffIndex = Index; }
	
};
