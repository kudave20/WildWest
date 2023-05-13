// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildWest/WildWestTypes/ScreenIndex.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Sheriff.generated.h"

#define ARM_LENGTH 200.f

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

	void SetInitialCharacter(int32 CurrentSheriffNum, bool bShouldStun);
	
	void StartStunTimer();

	UFUNCTION(Client, Reliable)
	void ClientStartStunTimer();

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void SwitchToFirst();
	void SwitchToSecond();
	void SwitchToThird();
	void SwitchToFourth();
	void Interact();

	void SwitchCharacter(int32 Index, EScreenIndex ScreenIndex);
	void ChangeHUDProperly(int32 Index, EScreenIndex ScreenIndex, EScreenIndex PreviousScreenIndex);
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
	void ServerInteract();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputConfigData* InputActions;

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	FRotator PreviousDirection;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	int32 CharacterIndex;

	UPROPERTY(EditAnywhere, Category = "Control")
	float InitialControlTimer;

	float ControlTimer;

	bool bIsInputEnabled = true;

	bool bIsAlone;

	UPROPERTY(EditDefaultsOnly, Category = "Control")
	float StunTimer;

	FTimerHandle TimerHandle;

	UPROPERTY()
	class ATownPlayerController* TownPlayerController;

	UPROPERTY()
	class UWildWestGameInstance* WildWestGameInstance;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UUserWidget> StunWidget;

	UPROPERTY()
	UUserWidget* Stun;

public:
	FORCEINLINE FRotator GetPreviousDirection() const { return PreviousDirection; }
	FORCEINLINE int32 GetCharacterIndex() const { return CharacterIndex; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};
