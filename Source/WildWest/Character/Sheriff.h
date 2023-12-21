// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Sheriff.generated.h"

#define ARM_LENGTH 150.f
#define FARSIGHTTHRESHOLDSQUARED 8000.f * 8000.f
#define NEARSIGHTTHRESHOLDSQUARED 2000.f * 2000.f

enum class EScreenIndex : uint8;

UCLASS()
class WILDWEST_API ASheriff : public ACharacter
{
	GENERATED_BODY()

public:
	ASheriff();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
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

	bool CanSwitch(int32 Index, EScreenIndex ScreenIndex);
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

	UPROPERTY(EditAnywhere, Category = "Control")
	float InitialControlTimer;

	float ControlTimer;

	bool bIsInputEnabled = true;

	UPROPERTY(EditAnywhere, Category = "Control")
	float StunTimer;

	FTimerHandle TimerHandle;

	UPROPERTY()
	class ATownPlayerController* TownPlayerController;

	UPROPERTY()
	class ATownGameState* TownGameState;

	UPROPERTY()
	class UWildWestGameInstance* WildWestGameInstance;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UUserWidget> StunWidget;

	UPROPERTY()
	UUserWidget* Stun;

	UPROPERTY()
	class AGunman* Gunman;

	bool TraceTowardGunman();

	EScreenIndex SheriffIndex;

	bool LineOfSightTo(AActor* OtherActor);

public:
	FORCEINLINE FRotator GetPreviousDirection() const { return PreviousDirection; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE void SetSheriffIndex(EScreenIndex NewIndex) { SheriffIndex = NewIndex; }
};
