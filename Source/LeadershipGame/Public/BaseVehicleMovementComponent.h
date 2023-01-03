// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "AIController.h"
#include "NavigationPath.h"
#include "BaseVehicleMovementComponent.generated.h"

USTRUCT()
struct FPIDController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "PID")
		float Proportional;

	UPROPERTY(EditAnywhere, Category = "PID")
		float Integral;

	UPROPERTY(EditAnywhere, Category = "PID")
		float Derivative;

	UPROPERTY(EditAnywhere, Category = "PID")
		float ErrorMin;

	UPROPERTY(EditAnywhere, Category = "PID")
		float ErrorMax;

	float ErrorSum;
	float LastPosition;

	FPIDController() {}

	FPIDController(float P, float I, float D, float ErrorMin, float ErrorMax)
	{
		Proportional = P;
		Integral = I;
		Derivative = D;
		this->ErrorMin = ErrorMin;
		this->ErrorMax = ErrorMax;
	}

	float CalcNewInput(float Error, float Position);
};

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API UBaseVehicleMovementComponent : public UChaosWheeledVehicleMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "PID")
	FPIDController ThrottleController = FPIDController(0, 0, 0, 0, 0);

	UPROPERTY(EditAnywhere, Category = "PID")
	FPIDController SteeringController = FPIDController(0, 0, 0, 0, 0);

	FVector InitialLocation;
	FVector InitialDirection;
	bool bTurningAround = false;

public:
	/*virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	virtual void StopActiveMovement() override;*/
	
};
