// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildWest/WildWestTypes/ScreenIndex.h"
#include "TownPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyDelegate);

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void ClientRemovePlayer();

	UFUNCTION(Server, Reliable)
	void ServerNotifyDisconnected();

	/*UFUNCTION(Client, Reliable)
	void ClientDestroySession();*/

	UPROPERTY(BlueprintAssignable)
	FOnNotifyDelegate NotifyDelegate;

	void InitialPossess();

	void SheriffHUDSetup(APawn* InPawn);
	void SetSheriffHUDScreen(int32 ScreenIndex);
	void SetSheriffHUDViewport(EScreenIndex ScreenIndex, EScreenIndex PreviousScreenIndex);
	void SetSheriffHUDGauge(float GaugePercent);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ShowReturnToMainMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY()
	class ATownGameState* TownGameState;

	EScreenIndex CurrentScreenIndex;

	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<AHUD> SheriffHUDClass;

	UPROPERTY()
	class ASheriffHUD* SheriffHUD;
	
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

	void PossessRandomly();

	bool bCompleteSetup;

public:
	FORCEINLINE EScreenIndex GetCurrentScreenIndex() const { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }
	FORCEINLINE bool IsSetupComplete() const { return bCompleteSetup; }
	FORCEINLINE void SetupComplete(bool bComplete) { bCompleteSetup = bComplete; }

};
