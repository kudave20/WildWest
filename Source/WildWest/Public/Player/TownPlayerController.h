// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildWestTypes/ScreenIndex.h"
#include "TownPlayerController.generated.h"

class ASheriff;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AVault;
class AGunmanHUD;
class UVaultGauge;
class ASheriffHUD;
class UReturnToMainMenu;
class ATownGameState;

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATownPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(Server, Reliable)
	void ServerNotifyDisconnected();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientCreateGunmanVictory();
	
	void InitialPossess();

	void SetGunmanPressWidget(AVault* TargetVault, TSubclassOf<UUserWidget> PressClass, bool bIsInteracting);
	void SetGunmanVaultGauge(AVault* TargetVault, TSubclassOf<UUserWidget> VaultGaugeClass, bool bIsInteracting);

	void SheriffHUDSetup();
	void SetSheriffHUDScreen(int32 ScreenIndex);
	void SetSheriffHUDViewport(int32 SheriffIndex);
	void SetSheriffHUDGauge(float GaugePercent);
	void StartStun();

	void HideSheriffHUDViewport();

	bool bCompleteSetup;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> TownContext;
	
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<UInputAction> OptionAction;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Jump();
	void ShowReturnToMainMenu();
	
	UPROPERTY()
	TObjectPtr<ATownGameState> TownGameState;

	EScreenIndex CurrentScreenIndex;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<AHUD> SheriffHUDClass;
	
	UPROPERTY()
	TObjectPtr<ASheriffHUD> SheriffHUD;
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> ReturnToMainMenuClass;

	UPROPERTY()
	TObjectPtr<UReturnToMainMenu> ReturnToMainMenu;

	UPROPERTY()
	TObjectPtr<UUserWidget> Press;
	
	UPROPERTY()
	TObjectPtr<UVaultGauge> VaultGauge;
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> StunClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> Stun;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> NotifyClass;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> GunmanVictoryClass;

	bool bReturnToMainMenuOpen = false;

	void PossessGunman();
	void PossessSheriff();

	bool bInputEnabled = true;

	UPROPERTY()
	TObjectPtr<ASheriff> Detector;

	float LastDetectedTime;

public:
	FORCEINLINE EScreenIndex GetCurrentScreenIndex() const { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }
	FORCEINLINE void SetInputEnabled(bool bEnabled) { bInputEnabled = bEnabled; }
	FORCEINLINE TObjectPtr<ASheriff> GetDetector() const { return Detector; }
	void Detect(ASheriff* InDetector, float InLastDetectedTime);
	FORCEINLINE bool IsDetecting() const { return LastDetectedTime > 0; }

};
