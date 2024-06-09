// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameViewportWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/LineBatchComponent.h"
#include "LegacyScreenPercentageDriver.h"
#include "EngineModule.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "CanvasTypes.h"

FCustomGameViewport::FCustomGameViewport(TWeakObjectPtr<UWorld> InWorld, const FPostProcessSettings& InPostProcessSettings) 
	: EngineShowFlags(ESFIM_Game)
	, World(InWorld)
	, PostProcessSettings(InPostProcessSettings)
{
	FSceneInterface* Scene = GetScene();
	ViewState.Allocate(Scene ? Scene->GetFeatureLevel() : GMaxRHIFeatureLevel);

	BackgroundColor = FColor(55, 55, 55);
}

FCustomGameViewport::~FCustomGameViewport()
{
	World.Reset();
}

void FCustomGameViewport::Draw(FViewport* InViewport, FCanvas* Canvas)
{
	FViewport* ViewportBackup = Viewport;
	Viewport = InViewport ? InViewport : Viewport;
	
	FGameTime Time = FGameTime::GetTimeSinceAppStart();

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		Canvas->GetRenderTarget(),
		GetScene(),
		EngineShowFlags)
		.SetTime(Time)
		.SetRealtimeUpdate(true));

	float GlobalResolutionFraction = GetDPIDerivedResolutionFraction();

	ViewFamily.EngineShowFlags.ScreenPercentage = true;
	
	FSceneView* View = CalcSceneView(&ViewFamily);
	
	ViewFamily.SetScreenPercentageInterface(new FLegacyScreenPercentageDriver(
		ViewFamily, GlobalResolutionFraction));

	View->CameraConstrainedViewRect = View->UnscaledViewRect;

	if (IsAspectRatioConstrained())
	{
		Canvas->Clear(FLinearColor::Black);
	}

	Canvas->Clear(BackgroundColor);

	--GFrameNumber;
	GetRendererModule().BeginRenderingViewFamily(Canvas, &ViewFamily);

	if (World == nullptr) return;

	if (World->LineBatcher != nullptr && (World->LineBatcher->BatchedLines.Num() || World->LineBatcher->BatchedPoints.Num()))
	{
		World->LineBatcher->Flush();
	}

	if (World->ForegroundLineBatcher != nullptr && (World->ForegroundLineBatcher->BatchedLines.Num() || World->ForegroundLineBatcher->BatchedPoints.Num()))
	{
		World->ForegroundLineBatcher->Flush();
	}

	Viewport = ViewportBackup;
}

FSceneView* FCustomGameViewport::CalcSceneView(FSceneViewFamily* ViewFamily)
{
	FSceneViewInitOptions ViewInitOptions;

	const APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	const FVector& ViewLocation = bUsePlayerCamLocation ? (PCM ? PCM->GetCameraLocation() : FVector::ZeroVector) : CameraLocation;
	const FRotator& ViewRotation = bUsePlayerCamRotation ? (PCM ? PCM->GetCameraRotation() : FRotator::ZeroRotator) : CameraRotation;

	const FIntPoint ViewportSizeXY = Viewport->GetSizeXY();

	FIntRect ViewRect = FIntRect(0, 0, ViewportSizeXY.X, ViewportSizeXY.Y);
	ViewInitOptions.SetViewRectangle(ViewRect);

	ViewInitOptions.ViewOrigin = ViewLocation;

	ViewInitOptions.ViewRotationMatrix = FInverseRotationMatrix(ViewRotation);
	ViewInitOptions.ViewRotationMatrix = ViewInitOptions.ViewRotationMatrix * FMatrix(
		FPlane(0, 0, 1, 0),
		FPlane(1, 0, 0, 0),
		FPlane(0, 1, 0, 0),
		FPlane(0, 0, 0, 1));

	const EAspectRatioAxisConstraint AspectRatioAxisConstraint = GetDefault<ULocalPlayer>()->AspectRatioAxisConstraint;

	FMinimalViewInfo::CalculateProjectionMatrixGivenView(ViewInfo, AspectRatioAxisConstraint, Viewport, ViewInitOptions);

	ViewInitOptions.ViewFamily = ViewFamily;
	ViewInitOptions.SceneViewStateInterface = ViewState.GetReference();
	ViewInitOptions.ViewElementDrawer = this;

	ViewInitOptions.BackgroundColor = GetBackgroundColor();
	FSceneView* View = new FSceneView(ViewInitOptions);
	ViewFamily->Views.Add(View);

	View->StartFinalPostprocessSettings(ViewLocation);
	View->EndFinalPostprocessSettings(ViewInitOptions);

	return View;
}

UWorld* FCustomGameViewport::GetWorld() const
{
	return World.Get();
}

FSceneInterface* FCustomGameViewport::GetScene() const
{
	return World.Get() ? World.Get()->Scene : nullptr;
}

FLinearColor FCustomGameViewport::GetBackgroundColor() const
{
	return BackgroundColor;
}

bool FCustomGameViewport::IsAspectRatioConstrained() const
{
	return ViewInfo.bConstrainAspectRatio;
}

void FCustomGameViewport::SetCameraLocation(const bool bUsePlayerCameraManagerLocation, const FVector InOverrideLocation)
{
	bUsePlayerCamLocation = bUsePlayerCameraManagerLocation;
	CameraLocation = InOverrideLocation;
}

void FCustomGameViewport::SetCameraRotation(const bool bUsePlayerCameraManagerRotation, const FRotator InOverrideRotation)
{
	bUsePlayerCamRotation = bUsePlayerCameraManagerRotation;
	CameraRotation = InOverrideRotation;
}

UGameViewportWidget::UGameViewportWidget()
{
	bIsActive = bShowOverlayText = true;
}

TSharedRef<SWidget> UGameViewportWidget::RebuildWidget()
{
	if (IsDesignTime())
	{
		return SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Game Viewport"))
			];
	}
	else
	{
		ViewportWidget = SNew(SCustomGameViewport).MyWorld(GetWorld());
		ViewportWidget->SetContentScale(FVector2D(10.f, 10.f));
		ViewportWidget->SetRenderDirectlyToWindow(false);
		ViewportWidget->SetActive(bIsActive);
		SetCameraLocation(true);
		SetCameraRotation(true);
		return SNew(SOverlay)
			+SOverlay::Slot()
			[
				ViewportWidget.ToSharedRef()
			];
	}
}

void UGameViewportWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ViewportWidget.Reset();
}

void UGameViewportWidget::SetCameraLocation(const bool bUsePlayerCameraManagerLocation /*= true*/, const FVector InOverrideLocation /*= FVector::ZeroVector*/)
{
	ViewportWidget->SetCameraLocation(bUsePlayerCameraManagerLocation, InOverrideLocation);
}

void UGameViewportWidget::SetCameraRotation(const bool bUsePlayerCameraManagerRotation /*= true*/, const FRotator InOverrideRotation /*= FRotator::ZeroRotator*/)
{
	ViewportWidget->SetCameraRotation(bUsePlayerCameraManagerRotation, InOverrideRotation);
}
