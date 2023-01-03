// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Templates/SubclassOf.h"
#include "BaseVehicleSeat.h"
#include "BaseVehicleMovementComponent.h"
#include "BaseVehiclePawn.generated.h"

UCLASS()
class LEADERSHIPGAME_API ABaseVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Seats")
		ABaseVehicleSeat* GetClosestSeat(const FVector Position);

public:
	// Sets default values for this character's properties
	ABaseVehiclePawn(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category = MapsAndSets, EditAnywhere, BlueprintReadWrite)
		TMap<FName, TSubclassOf<ABaseVehicleSeat>> SeatMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ABaseVehicleSeat*> Seats;

	UFUNCTION(BlueprintCallable, Category = "Seats")
		bool EnterVehicle(APawn* Pawn);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WheelSetup)
		TSubclassOf<UChaosVehicleWheel> FrontWheelClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WheelSetup)
		TSubclassOf<UChaosVehicleWheel> BackWheelClass;
};
