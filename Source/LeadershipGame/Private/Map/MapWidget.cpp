// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapWidget.h"
#include "Map/MapIconWidget.h"

UMapWidget::UMapWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMapWidget::OnBeginPlay()
{
	CaptureComponent = nullptr;
	TArray<AActor*> ActorsFound;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASceneCapture2D::StaticClass(), ActorsFound);
	for (AActor* ActorFound : ActorsFound)
	{
		ASceneCapture2D* SceneCapture = Cast<ASceneCapture2D>(ActorFound);
		if (SceneCapture && SceneCapture->GetCaptureComponent2D())
		{
			CaptureComponent = SceneCapture->GetCaptureComponent2D();
			MaxOrthoWidth = CaptureComponent->OrthoWidth;

			break;
		}
	}

	if (CaptureComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MapWidget could not find ASceneCapture2D"));
	}


	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("OrthoWidth: %f"), CaptureComponent->OrthoWidth));
}

void UMapWidget::UpdateAllIcons()
{
	for (UMapIconWidget* Icon : Icons)
	{
		Icon->UpdatePosition();
		//UE_LOG(LogTemp, Warning, TEXT("Box: %d"), *GetBox2D().ToString());
	}
}

void UMapWidget::UpdatePendingIcons()
{

	for (UMapIconWidget* Icon : Icons)
	{
		if(Icon->ShouldUpdatePosition()) {
			Icon->UpdatePosition();
		}
	}
}

FVector UMapWidget::FindClickedLocation(const FPointerEvent& MouseEvent, bool& DidHitOUT)
{
	const FVector2D MousePosition = MouseEvent.GetScreenSpacePosition();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "MousePosition: " + MousePosition.ToString());
	const FVector2D MapPoint = ScreenToMapPoint(MousePosition);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "MousePositionReversed: " + MapToScreenPoint(MapPoint).ToString());
	return MapToWorldPoint(MapPoint, DidHitOUT);
}

FVector2D UMapWidget::ScreenToMapPoint(const FVector2D ScreenPoint)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "ScreenPoint: " + ScreenPoint.ToString());
	const FBox2D MapBounds = GetBox2D();

	FVector2D InvMapPoint = (ScreenPoint - MapBounds.GetCenter()) / MapBounds.GetExtent();
	return FVector2D(-InvMapPoint.Y, InvMapPoint.X);
}
             
FVector2D UMapWidget::MapToScreenPoint(const FVector2D MapPoint)
{
	const FBox2D MapBounds = GetBox2D();

	FVector2D InvMapPoint = FVector2D(MapPoint.Y, -MapPoint.X);
	return MapBounds.GetCenter() + (InvMapPoint * MapBounds.GetExtent());
}

FVector UMapWidget::MapToWorldPoint(const FVector2D MapPoint, bool& DidHitOUT)
{
	FVector2D WorldVector2D = MapPoint / 2 * CaptureComponent->OrthoWidth;

	FHitResult HitDetails = FHitResult(ForceInit);
	FCollisionQueryParams TraceParams(FName(TEXT("InteractMapTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	DidHitOUT = GetWorld()->LineTraceSingleByChannel(
		HitDetails,      // FHitResult object that will be populated with hit info
		FVector(WorldVector2D, 5000.0),      // starting position
		FVector(WorldVector2D, -5000.0),        // end position
		ECC_Visibility,  // collision channel - 3rd custom one
		TraceParams      // additional trace settings
	);

	DrawDebugLine(GetWorld(), FVector(WorldVector2D, 5000.0), FVector(WorldVector2D, -5000.0), FColor::Purple, false, 120.f, ECC_WorldStatic, 4.f);

	if (DidHitOUT) {
		DrawDebugBox(GetWorld(), HitDetails.Location, FVector(20.f, 20.f, 20.f), FColor::Blue, false, 120.f, ECC_WorldStatic, 2.f);
		return HitDetails.Location;
	}
	else {
		return FVector();
	}
}


FVector2D UMapWidget::WorldToMapPoint(const FVector WorldPoint)
{
	FVector CaptureOffset = WorldPoint - CaptureComponent->GetComponentLocation();
	return FVector2D(CaptureOffset.X * 2 / CaptureComponent->OrthoWidth, CaptureOffset.Y * 2 / CaptureComponent->OrthoWidth);
}

//FVector UMapWidget::MapToWorldPoint(const FVector2D MapPoint, USceneCaptureComponent2D* CaptureComponent)
//{
//	FMinimalViewInfo ViewInfo;
//	CaptureComponent->GetCameraView(-1.f, ViewInfo);
//
//	FVector WorldPosition;
//	FVector WorldDirection;
//	FMatrix const InvViewProjMatrix = ViewInfo.CalculateProjectionMatrix().InverseFast();
//
//	int32 X = FMath::TruncToInt(CaptureComponent->GetComponentLocation().X * 512.f);
//	int32 Y = FMath::TruncToInt(CaptureComponent->GetComponentLocation().Y * 512.f);
//	uint32 SizeX = 512;
//	uint32 SizeY = 512;
//	FIntRect UnconstrainedRectangle = FIntRect(X, Y, X + SizeX, Y + SizeY);
//
//	FSceneView::DeprojectScreenToWorld(MapPoint, UnconstrainedRectangle, InvViewProjMatrix, /*out*/ WorldPosition, /*out*/ WorldDirection);
//}

float UMapWidget::GetZoom()
{
	return Zoom;
}

void UMapWidget::SetZoom(float NextZoom)
{
	NextZoom = FMath::Clamp(NextZoom, 1.0f, MaxZoom);
	SetRenderScale(FVector2D(NextZoom));
	SetRenderTranslation((RenderTransform.Translation / Zoom) * NextZoom);
	Zoom = NextZoom;
}

FBox2D UMapWidget::GetBox2D()
{
	FGeometry MapGeometry = GetCachedGeometry();
	const FVector2D BoxMin = MapGeometry.GetRenderBoundingRect().GetTopLeft(); //MapGeometry.LocalToAbsolute();
	const FVector2D BoxMax = BoxMin + MapGeometry.GetAbsoluteSize();

	return FBox2D(BoxMin, BoxMax);
}

UCanvasPanelSlot* UMapWidget::GetCanvasSlot()
{
	return Cast<UCanvasPanelSlot>(Slot.Get());
}

