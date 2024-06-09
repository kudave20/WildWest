#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_None UMETA(DisplayName = "None"),
	ECS_Gunman UMETA(DisplayName = "Gunman"),
	ECS_Sheriff UMETA(DisplayName = "Sheriff"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

