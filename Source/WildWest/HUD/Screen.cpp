// Fill out your copyright notice in the Description page of Project Settings.


#include "Screen.h"
#include "Components/TextBlock.h"
#include "WildWest/Character/Sheriff.h"

bool UScreen::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Screen)
	{
		Screen->TextDelegate.BindUFunction(this, "SetScreen");
	}

	return true;
}

FText UScreen::SetScreen()
{
	ASheriff* Sheriff = Cast<ASheriff>(GetOwningPlayerPawn());
	if (Sheriff)
	{
		int32 Index = Sheriff->GetCharacterIndex();
		FString Result = FString::FromInt(Index);
		return FText::FromString(Result);
	}

	return FText::FromString("NULL");
}
