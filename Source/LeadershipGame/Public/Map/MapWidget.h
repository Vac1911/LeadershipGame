// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/Box2D.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Layout/Geometry.h"
#include "MapWidget.generated.h"

class UMapIconWidget;

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API UMapWidget : public UImage
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	USceneCaptureComponent2D* CaptureComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance")
	FButtonStyle IconStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMapIconWidget*> SelectedIcons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMapIconWidget*> Icons;

protected:
	float MaxOrthoWidth;

	float Zoom = 1.0f;

	float MaxZoom = 8.0f;

public:
	/*
	 * This needs to be called manually after the parent UUserWidget::BeginPlay()
	 */
	UFUNCTION(BlueprintCallable, Category = "Map")
	void OnBeginPlay();

	/**
	* Update all icons.
	*/
	UFUNCTION(BlueprintCallable, Category = "Map")
	void UpdateAllIcons();

	/**
	* Update icons which are pendng update.
	*/
	UFUNCTION(BlueprintCallable, Category = "Map")
	void UpdatePendingIcons();

	/**
	* Find Clicked World Point
	*/
	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector	FindClickedLocation(const FPointerEvent& MouseEvent, bool& DidHitOUT);

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector2D ScreenToMapPoint(const FVector2D ScreenPoint);

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector2D MapToScreenPoint(const FVector2D MapPoint);

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector MapToWorldPoint(const FVector2D MapPoint, bool& DidHitOUT);

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector2D WorldToMapPoint(const FVector WorldPoint);

	UFUNCTION(BlueprintCallable, Category = "Map")
	float GetZoom();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void SetZoom(float NextZoom);

	FBox2D GetBox2D();

	UCanvasPanelSlot* GetCanvasSlot();
};
