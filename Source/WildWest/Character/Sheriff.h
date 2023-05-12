// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WildWest/WildWestTypes/ScreenIndex.h"
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

	UFUNCTION(BlueprintCallable)
	void SelectControlCharacter();
	
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
	void EnterDuel();

	void SwitchCharacter(int32 Index, EScreenIndex ScreenIndex);
	void ChangeHUDProperly(int32 Index, EScreenIndex ScreenIndex);
	
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

	UPROPERTY(Replicated, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	int32 CharacterIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Control")
	float InitialControlTimer;

	float ControlTimer;

	bool bIsInputEnabled{ true };

	UPROPERTY(Replicated)
	bool bIsAlone;

	UPROPERTY()
	class AGunman* OverlappingGunman;

	UPROPERTY(EditDefaultsOnly, Category = "Control")
	float StunTimer;

	FTimerHandle TimerHandle;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	class ATownPlayerController* TownPlayerController;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UUserWidget> StunWidget;

	UPROPERTY()
	UUserWidget* Stun;

public:
	FORCEINLINE FRotator GetControllerDirection() { return ControllerDirection; }
	FORCEINLINE void SetControllerDirection(FRotator NewControllerDirection) { ControllerDirection = NewControllerDirection; }
	FORCEINLINE int32 GetCharacterIndex() { return CharacterIndex; }
	FORCEINLINE void SetOverlappingGunman(AGunman* Gunman) { OverlappingGunman = Gunman; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};
