// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitVehiclePawn.h"

FName AUnitVehiclePawn::VehicleNavComponentName(TEXT("VehicleNavComp"));

AUnitVehiclePawn::AUnitVehiclePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NavComponent = CreateDefaultSubobject<UMercunaGroundNavigationComponent>(VehicleNavComponentName);
	AIControllerClass = AUnitAIController::StaticClass();
}

void AUnitVehiclePawn::BeginPlay()
{
}

TArray<FVector> AUnitVehiclePawn::GetWaypoints_Implementation()
{
	return TArray<FVector>();
}

AUnitAIController* AUnitVehiclePawn::GetUnitController()
{
	return Cast<AUnitAIController>(GetController());
}
