// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "Widgets/SViewport.h"
#include "Slate/SceneViewport.h"
#include "SceneManagement.h"
#include "UnrealClient.h"
#include "GameViewportWidget.generated.h"

class WILDWEST_API FCustomGameViewport : public FCommonViewportClient, public FViewElementDrawer
{
public:
	FCustomGameViewport(TWeakObjectPtr<UWorld> InWorld, const FPostProcessSettings& InPostProcessSettings);
	~FCustomGameViewport();

private:
	FViewport* Viewport;
	FSceneViewStateReference ViewState;
	FMinimalViewInfo ViewInfo;
	FEngineShowFlags EngineShowFlags;
	FLinearColor BackgroundColor;
	TWeakObjectPtr<UWorld> World;
	FPostProcessSettings PostProcessSettings;

	uint8 bUsePlayerCamLocation : 1;
	uint8 bUsePlayerCamRotation : 1;

	FVector CameraLocation;
	FRotator CameraRotation;

public:
	using FViewElementDrawer::Draw;

	virtual FSceneView* CalcSceneView(FSceneViewFamily* ViewFamily);
	virtual void Draw(FViewport* InViewport, FCanvas* Canvas) override;
	virtual UWorld* GetWorld() const override;
	virtual FSceneInterface* GetScene() const;
	FLinearColor GetBackgroundColor() const;
	bool IsAspectRatioConstrained() const;

	void SetCameraLocation(const bool bUsePlayerCameraManagerLocation, const FVector InOverrideLocation);
	void SetCameraRotation(const bool bUsePlayerCameraManagerRotation, const FRotator InOverrideRotation);
};

class SCustomGameViewport : public SViewport
{
	SLATE_BEGIN_ARGS(SCustomGameViewport)
		: _MyWorld(nullptr)
	{
	}

	SLATE_ARGUMENT(UWorld*, MyWorld)
	SLATE_ARGUMENT(FPostProcessSettings, PostProcessSettings)
	SLATE_END_ARGS()

	SCustomGameViewport()
	{
	}

	void Construct(const FArguments& InArgs)
	{
		SViewport::FArguments ParentArgs;
		ParentArgs.IgnoreTextureAlpha(false);
		ParentArgs.EnableStereoRendering(false);
		SViewport::Construct(ParentArgs);

		ViewportClient = MakeShareable(new FCustomGameViewport(InArgs._MyWorld, InArgs._PostProcessSettings));
		Viewport = MakeShareable(new FSceneViewport(ViewportClient.Get(), SharedThis(this)));

		SetViewportInterface(Viewport.ToSharedRef());
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		if (IsEnabled())
		{
			Viewport->Invalidate();
			Viewport->Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
		}
	}

	FORCEINLINE void SetCameraLocation(const bool bUsePlayerCameraManagerLocation, const FVector InOverrideLocation)
	{
		ViewportClient->SetCameraLocation(bUsePlayerCameraManagerLocation, InOverrideLocation);
	}

	FORCEINLINE void SetCameraRotation(const bool bUsePlayerCameraManagerRotation, const FRotator InOverrideRotation)
	{
		ViewportClient->SetCameraRotation(bUsePlayerCameraManagerRotation, InOverrideRotation);
	}

public:
	TSharedPtr<FCustomGameViewport> ViewportClient;
	TSharedPtr<FSceneViewport> Viewport;

	FText GetOverlayText() const { return FText::GetEmpty(); }
};

UCLASS()
class WILDWEST_API UGameViewportWidget : public UContentWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Game Viewport Widget")
	bool bIsActive;

	UPROPERTY(EditAnywhere, Category = "Game Viewport Widget")
	bool bShowOverlayText;

protected:
	TSharedPtr<class SCustomGameViewport> ViewportWidget;

public:
	UGameViewportWidget();

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void SetCameraLocation(const bool bUsePlayerCameraManagerLocation = true, const FVector InOverrideLocation = FVector::ZeroVector);
	void SetCameraRotation(const bool bUsePlayerCameraManagerRotation = true, const FRotator InOverrideRotation = FRotator::ZeroRotator);
	
};
