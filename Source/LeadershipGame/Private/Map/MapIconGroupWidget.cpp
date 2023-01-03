// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapIconGroupWidget.h"

UMapIconGroupWidget* UMapIconGroupWidget::CreateGroupIcon(UUserWidget* InWidget, UMapWidget* MapWidget, UUnitGroup* Group)
{
	if ((!InWidget) || (!MapWidget))
	{
		UE_LOG(LogTemp, Warning, TEXT("UMapIconGroupWidget::CreateGroupIcon was not passed InWidget and/or MapWidget"));
		return nullptr;
	}

	IUnitInterface* Unit = Cast<IUnitInterface>(Group);
	if (!Unit)
	{
		return nullptr;
	}

	TObjectPtr<UWidgetTree> WidgetTree = InWidget->WidgetTree;
	UMapIconGroupWidget* CreatedWidget = UMapIconWidget::ConstructIcon<UMapIconGroupWidget>(InWidget, MapWidget);
	CreatedWidget->FollowedUnit.SetObject(Group);
	CreatedWidget->UpdatePosition();

	return CreatedWidget;
}