// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Unit/UnitGroup.h"
#include "UnitInterface.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UUnitGroup*> UnitGroups;

	void AddGroup(UUnitGroup* Group);

	UUnitGroup* SpawnFireteam(FVector Location, TSubclassOf<APawn> UnitType);

	UUnitGroup* SpawnVehicle(FVector Location, TSubclassOf<APawn> UnitType);
};
