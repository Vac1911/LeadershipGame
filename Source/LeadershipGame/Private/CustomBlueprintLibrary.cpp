// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomBlueprintLibrary.h"

UWidget* UCustomBlueprintLibrary::CreateChildWidget(UUserWidget* InWidget, TSubclassOf<UWidget> WidgetToCreate)
{
	if ((!InWidget) || (!WidgetToCreate))
	{
		return nullptr;
	}
	TObjectPtr<UWidgetTree> WidgetTree = InWidget->WidgetTree;
	UWidget* CreatedWidget = WidgetTree->ConstructWidget<UWidget>(WidgetToCreate);
	return CreatedWidget;
}

void UCustomBlueprintLibrary::IssueOrderToUnit(TScriptInterface<IUnitInterface> Unit, URTSOrder* Order)
{
    if (!IsValid(Unit.GetObject()) || !IsValid(Order))
    {
        return;
    }
    Order->IssueOrder(Unit);
}