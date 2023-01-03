// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapIconBaseUnitWidget.h"
#include "Unit/UnitGroup.h"
#include "MapIconGroupWidget.generated.h"

/**
 *
 */
UCLASS()
class LEADERSHIPGAME_API UMapIconGroupWidget : public UMapIconBaseUnitWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Map")
	static UMapIconGroupWidget* CreateGroupIcon(UUserWidget* InWidget, UMapWidget* MapWidget, UUnitGroup* Group);
};
