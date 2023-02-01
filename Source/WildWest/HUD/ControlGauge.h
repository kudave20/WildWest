// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlGauge.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UControlGauge : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Gauge;

public:
	FORCEINLINE UImage* GetGauge() { return Gauge; }

};
