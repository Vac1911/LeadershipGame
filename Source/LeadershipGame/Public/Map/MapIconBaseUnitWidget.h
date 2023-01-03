// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitInterface.h"
#include "Map/MapIconWidget.h"
#include "Blueprint/WidgetTree.h"
#include "MapIconBaseUnitWidget.generated.h"

/**
 *
 */
UCLASS()
class LEADERSHIPGAME_API UMapIconBaseUnitWidget : public UMapIconWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	TScriptInterface<IUnitInterface> FollowedUnit;

	UFUNCTION(BlueprintCallable, Category = "Map")
	FVector GetUnitPosition();

	UFUNCTION(BlueprintCallable, Category = "Map")
	virtual bool ShouldUpdatePosition() override;

	UFUNCTION(BlueprintCallable, Category = "Map")
	virtual void UpdatePosition() override;
};
