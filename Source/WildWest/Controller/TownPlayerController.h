// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TownPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyDelegate);

UENUM(BlueprintType)
enum class EScreenIndex : uint8
{
	ECI_First UMETA(DisplayName = "First Screen"),
	ECI_Second UMETA(DisplayName = "Second Screen"),
	ECI_Third UMETA(DisplayName = "Third Screen"),
	ECI_Fourth UMETA(DisplayName = "Fourth Screen"),

	ECI_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * 
 */
UCLASS()
class WILDWEST_API ATownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetInitialControlRotation(const FRotator& NewRotation);

	UFUNCTION(Client, Reliable)
	void ClientRemovePlayer();

	UFUNCTION(Server, Reliable)
	void ServerNotifyDisconnected();

	/*UFUNCTION(Client, Reliable)
	void ClientDestroySession();*/

	UPROPERTY(BlueprintAssignable)
	FOnNotifyDelegate NotifyDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void ShowReturnToMainMenu();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	EScreenIndex CurrentScreenIndex;
	
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UUserWidget> ReturnToMainMenuWidget;

	UPROPERTY()
	class UReturnToMainMenu* ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

public:
	FORCEINLINE EScreenIndex GetCurrentScreenIndex() { return CurrentScreenIndex; }
	FORCEINLINE void SetCurrentScreenIndex(EScreenIndex NewScreenIndex) { CurrentScreenIndex = NewScreenIndex; }

};
