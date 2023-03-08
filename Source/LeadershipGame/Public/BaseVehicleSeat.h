// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BaseVehicleSeat.generated.h"


UCLASS()
class LEADERSHIPGAME_API ABaseVehicleSeat : public APawn
{
	GENERATED_BODY()

public:
	ABaseVehicleSeat();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* CurrentPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWheeledVehiclePawn* Vehicle;

	UFUNCTION(BlueprintCallable, Category = "Seat")
	bool IsOccupied();

	UFUNCTION(BlueprintCallable, Category = "Seat")
	virtual void EnterSeat(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = "Seat")
	virtual void LeaveSeat();
};
