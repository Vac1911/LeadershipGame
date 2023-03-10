// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UnitInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class LEADERSHIPGAME_API IUnitInterface
{
	GENERATED_BODY()

public:
	/** Get location of unit */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	FVector GetLocation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	TArray<FVector> GetWaypoints();

	/** Get current order */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	UObject* GetCurrentOrder();

	/** Stops the unit's current order */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	void StopCurrentOrder();

	/** Is unit idle? */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	bool IsIdle();

	/** Is the current order complete? */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	bool GetOrderIsComplete();

	/** Set the current order complete? */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	void SetOrderIsComplete(bool IsComplete);

	/*UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	UObject* GetParentGroup();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit")
	void SetParentGroup(UObject* Parent);*/
};
