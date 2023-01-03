// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapIconBaseUnitWidget.h"

FVector UMapIconBaseUnitWidget::GetUnitPosition()
{
	return IUnitInterface::Execute_GetLocation(FollowedUnit.GetObject());
}

bool UMapIconBaseUnitWidget::ShouldUpdatePosition()
{
	return Super::ShouldUpdatePosition() || (GetUnitPosition() != WorldLocation);
}

void UMapIconBaseUnitWidget::UpdatePosition()
{
	WorldLocation = GetUnitPosition();
	SetMapPoint(ParentMap->WorldToMapPoint(WorldLocation));
	Super::UpdatePosition();
}