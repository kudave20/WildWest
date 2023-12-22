// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TownPlayerController.generated.h"

enum class EScreenIndex : uint8;
class UInputMappingContext;
class UInputConfigData;
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
	UFUNCTION(Server, Reliable)
	void ServerNotifyDisconnected();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientCreateGunmanVictory();
	
	void InitialPossess();

	void SheriffHUDSetup(APawn* InPawn);
	void SetSheriffHUDScreen(int32 ScreenIndex);
	void SetSheriffHUDViewport(EScreenIndex ScreenIndex, EScreenIndex PreviousScreenIndex);
	void SetSheriffHUDGauge(float GaugePercent);

	void SetSheriffViewportRendered(EScreenIndex ScreenIndex, bool bRendered);
	void DrawSheriffViewport(EScreenIndex ScreenIndex);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ShowReturnToMainMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputConfigData> InputActions;

private:
	UPROPERTY()
	TObjectPtr<ATownGameState> TownGameState;

	EScreenIndex CurrentScreenIndex;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<AHUD> SheriffHUDClass;

	UPROPERTY()
	ASheriffHUD* SheriffHUD;
	
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> ReturnToMainMenuClass;

	UPROPERTY()
	UReturnToMainMenu* ReturnToMainMenu;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> NotifyClass;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> GunmanVictoryClass;

	bool bReturnToMainMenuOpen = false;

	void PossessGunman();
	void PossessSheriffRandomly();

	bool bCompleteSetup;

public:
	FORCEINLINE EScreenIndex GetCurrentScreenIndex() const { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }
	FORCEINLINE bool IsSetupComplete() const { return bCompleteSetup; }
	FORCEINLINE void SetupComplete(bool bComplete) { bCompleteSetup = bComplete; }

};
