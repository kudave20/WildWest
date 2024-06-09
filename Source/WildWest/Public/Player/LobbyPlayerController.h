// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WildWestTypes/CharacterState.h"
#include "LobbyPlayerController.generated.h"

class UReturnToMainMenu;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCharacterSelect;

/**
 * 
 */
UCLASS()
class WILDWEST_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void GunmanButtonClicked();
	UFUNCTION(Server, Reliable)
	void ServerGunmanButtonClicked();

	void SheriffButtonClicked();
	UFUNCTION(Server, Reliable)
	void ServerSheriffButtonClicked();

	void AddCharacterSelect();

	void ChangeButtonState(ECharacterState NewState);

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
	
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<UUserWidget> CharacterSelectClass;

	UPROPERTY()
	TObjectPtr<UCharacterSelect> CharacterSelect;

	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> ReturnToMainMenuClass;

	UPROPERTY()
	TObjectPtr<UReturnToMainMenu> ReturnToMainMenu;

	bool bReturnToMainMenuOpen = false;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Jump();
	void ShowReturnToMainMenu();
};
