// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "WheeledVehiclePawn.h"
#include "UnitInterface.h"
#include "UnitAIController.h"
#include "Components/MercunaGroundNavigationComponent.h"
#include "UnitVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API AUnitVehiclePawn : public AWheeledVehiclePawn, public IUnitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitVehiclePawn(const FObjectInitializer& ObjectInitializer);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** vehicle navigation component */
	class UMercunaGroundNavigationComponent* NavComponent;


	/** Name of the UMercunaGroundNavigationComponent. */
	static FName VehicleNavComponentName;

	TArray<FVector> GetWaypoints_Implementation() override;

	AUnitAIController* GetUnitController();
};
