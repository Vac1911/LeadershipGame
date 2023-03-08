// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "UnitInterface.h"
#include "UnitAIController.h"
#include "Unit/UnitGroup.h"
#include "Templates/SubclassOf.h"
#include "BaseVehicleSeat.h"
#include "BaseVehicleMovementComponent.h"
#include "UnitCharacter.h"
#include "BaseVehiclePawn.generated.h"

UCLASS()
class LEADERSHIPGAME_API ABaseVehiclePawn : public AWheeledVehiclePawn, public IUnitInterface
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

	UFUNCTION(BlueprintCallable, Category = "Seats")
	bool EnterVehicleSeat(APawn* Pawn, ABaseVehicleSeat* Seat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WheelSetup)
	TSubclassOf<UChaosVehicleWheel> FrontWheelClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WheelSetup)
	TSubclassOf<UChaosVehicleWheel> BackWheelClass;

	/*
	* Unit Interface
	*/
	UUnitGroup* ParentGroup;

	bool HasCompletedOrder = false;

	AUnitAIController* GetUnitController();

	FVector GetLocation_Implementation() override;

	UObject* GetCurrentOrder_Implementation() override;

	/** Remove Current Order */
	void StopCurrentOrder_Implementation() override;

	/** Currently Unit is idle */
	bool IsIdle_Implementation() override;

	/** Is the current order complete? */
	bool GetOrderIsComplete_Implementation() override;

	void SetOrderIsComplete_Implementation(bool IsComplete) override;

	UObject* GetParentGroup_Implementation() /*override*/;

	void SetParentGroup_Implementation(UObject* Parent) /*override*/;
};
