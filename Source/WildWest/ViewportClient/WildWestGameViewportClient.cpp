// Fill out your copyright notice in the Description page of Project Settings.


#include "WildWestGameViewportClient.h"

UWildWestGameViewportClient::UWildWestGameViewportClient(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SplitscreenInfo[ESplitScreenType::FourPlayer_Grid].PlayerData.Empty();

	SplitscreenInfo[ESplitScreenType::FourPlayer_Grid].PlayerData.Add(FPerPlayerSplitscreenData(0.667f, 1.f, 0.0f, 0.0f));
	SplitscreenInfo[ESplitScreenType::FourPlayer_Grid].PlayerData.Add(FPerPlayerSplitscreenData(0.333f, 0.333f, 0.667f, 0.0f));
	SplitscreenInfo[ESplitScreenType::FourPlayer_Grid].PlayerData.Add(FPerPlayerSplitscreenData(0.333f, 0.333f, 0.667f, 0.333f));
	SplitscreenInfo[ESplitScreenType::FourPlayer_Grid].PlayerData.Add(FPerPlayerSplitscreenData(0.333f, 0.334f, 0.667f, 0.666f));
}