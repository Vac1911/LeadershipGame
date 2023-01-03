// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapIconUnitWidget.h"

UMapIconUnitWidget* UMapIconUnitWidget::CreateUnitIcon(UUserWidget* InWidget, UMapWidget* MapWidget, AActor* Followed)
{
	if ((!InWidget) || (!Followed) || (!MapWidget))
	{
		return nullptr;
	}
	IUnitInterface* Unit = Cast<IUnitInterface>(Followed);
	if(!Unit)
	{
		return nullptr;
	}

	TObjectPtr<UWidgetTree> WidgetTree = InWidget->WidgetTree;
	UMapIconUnitWidget* CreatedWidget = UMapIconWidget::ConstructIcon<UMapIconUnitWidget>(InWidget, MapWidget);
	CreatedWidget->FollowedUnit.SetObject(Followed);
	CreatedWidget->UpdatePosition();

	return CreatedWidget;
}
