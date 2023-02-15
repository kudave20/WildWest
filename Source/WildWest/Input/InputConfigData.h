// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInput/Public/InputAction.h"
#include "InputConfigData.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UInputConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputJump;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputMoveForward;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputMoveRight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputTurn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputLookUp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSwitchToFirst;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSwitchToSecond;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSwitchToThird;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputSwitchToFourth;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputInteract;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputAction* InputOption;
};
