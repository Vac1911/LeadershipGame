// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h" 
#include "Blueprint/WidgetTree.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "UnitInterface.h"
#include "CustomBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API UCustomBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Creates a base UWidget (text, border, image, etc) at runtime. */
	static UWidget* CreateChildWidget(UUserWidget* InWidget, TSubclassOf<UWidget> WidgetToCreate);

	/** Issues the specified order to the passed actor. */
	UFUNCTION(BlueprintCallable, Category = "Unit|Order")
	static void IssueOrderToUnit(TScriptInterface<IUnitInterface> Unit, URTSOrder* Order);
};
