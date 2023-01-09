// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "WildWestGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class WILDWEST_API UWildWestGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	
public:
	UWildWestGameViewportClient(const FObjectInitializer& ObjectInitializer);
};
