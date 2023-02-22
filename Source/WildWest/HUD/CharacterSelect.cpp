// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelect.h"
#include "Components/Button.h"
#include "WildWest/GameState/LobbyGameState.h"
#include "WildWest/Controller/LobbyPlayerController.h"

void UCharacterSelect::CharacterSelectSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if (LobbyPlayerController)
	{
		LobbyPlayerController->SelectServerCharacterDelegate.AddUObject(this, &ThisClass::OnChangeServerCharacter);
		LobbyPlayerController->SelectClientCharacterDelegate.AddUObject(this, &ThisClass::OnChangeClientCharacter);
	}
}

bool UCharacterSelect::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (GunmanButton)
	{
		GunmanButton->OnClicked.AddDynamic(this, &ThisClass::GunmanButtonClicked);
	}
	if (SheriffButton)
	{
		SheriffButton->OnClicked.AddDynamic(this, &ThisClass::SheriffButtonClicked);
	}

	return true;
}

void UCharacterSelect::GunmanButtonClicked()
{
	GunmanButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->GunmanButtonClicked();
		}
	}

	SheriffButton->SetIsEnabled(true);
}

void UCharacterSelect::SheriffButtonClicked()
{
	SheriffButton->SetIsEnabled(false);

	UWorld* World = GetWorld();
	if (World)
	{
		ALobbyGameState* LobbyGameState = World->GetGameState<ALobbyGameState>();
		if (LobbyGameState)
		{
			LobbyGameState->SheriffButtonClicked();
		}
	}

	GunmanButton->SetIsEnabled(true);
}

void UCharacterSelect::OnChangeServerCharacter(ECharacterState NewCharacterState)
{
	switch (NewCharacterState)
	{
	case ECharacterState::ECS_Gunman:
		GunmanButton->SetIsEnabled(false);
		SheriffButton->SetIsEnabled(true);
		break;
	case ECharacterState::ECS_Sheriff:
		SheriffButton->SetIsEnabled(false);
		GunmanButton->SetIsEnabled(true);
		break;
	}
}

void UCharacterSelect::OnChangeClientCharacter(ECharacterState NewCharacterState)
{
	switch (NewCharacterState)
	{
	case ECharacterState::ECS_Gunman:
		GunmanButton->SetIsEnabled(false);
		SheriffButton->SetIsEnabled(true);
		break;
	case ECharacterState::ECS_Sheriff:
		SheriffButton->SetIsEnabled(false);
		GunmanButton->SetIsEnabled(true);
		break;
	}
}
