// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapIconBaseUnitWidget.h"
#include "MapIconUnitWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API UMapIconUnitWidget : public UMapIconBaseUnitWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Map")
	static UMapIconUnitWidget* CreateUnitIcon(UUserWidget* InWidget, UMapWidget* MapWidget, AActor* Followed);
};
