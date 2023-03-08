// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseVehiclePawn.h"

ABaseVehiclePawn::ABaseVehiclePawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseVehicleMovementComponent>(VehicleMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	UBaseVehicleMovementComponent* VehicleMovement = CastChecked<UBaseVehicleMovementComponent>(GetVehicleMovementComponent());
	if (VehicleMovement)
	{
		VehicleMovement->Mass = 3200.f;
		VehicleMovement->DragCoefficient = 0.3f;
		VehicleMovement->ChassisWidth = 180.f;
		VehicleMovement->ChassisHeight = 140.f;
		VehicleMovement->InertiaTensorScale = FVector(4.f, 4.f, 6.f);

		VehicleMovement->TransmissionSetup.GearChangeTime = 0.5f;
		VehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
		VehicleMovement->EngineSetup.MaxRPM = 3400.0f;

		FRichCurve* TorqueCurveData = VehicleMovement->EngineSetup.TorqueCurve.GetRichCurve();
		TorqueCurveData->Reset();
		TorqueCurveData->AddKey(0.0f, 400.0f);
		TorqueCurveData->AddKey(1150.0f, 470.0f);
		TorqueCurveData->AddKey(2450.0f, 462.0f);
		TorqueCurveData->AddKey(3220.0f, 415.0f);

		VehicleMovement->WheelSetups.AddUninitialized(4);
		{
			VehicleMovement->WheelSetups[0].WheelClass = FrontWheelClass;
			VehicleMovement->WheelSetups[0].BoneName = FName(TEXT("FL"));

			VehicleMovement->WheelSetups[1].WheelClass = FrontWheelClass;
			VehicleMovement->WheelSetups[1].BoneName = FName(TEXT("FR"));

			VehicleMovement->WheelSetups[2].WheelClass = BackWheelClass;
			VehicleMovement->WheelSetups[2].BoneName = FName(TEXT("RL"));

			VehicleMovement->WheelSetups[3].WheelClass = BackWheelClass;
			VehicleMovement->WheelSetups[3].BoneName = FName(TEXT("RR"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("No VehicleMovement Comp"));
	}


	AIControllerClass = AUnitAIController::StaticClass();
}

void ABaseVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* VehicleMesh = GetMesh();
	FActorSpawnParameters Parameters;
	Parameters.Owner = this;
	Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (auto It = SeatMap.CreateConstIterator(); It; ++It)
	{
		FName SocketName = It.Key();
		TSubclassOf<ABaseVehicleSeat> SeatClass = It.Value().Get();

		FVector SpawnLocation = VehicleMesh->GetSocketLocation(SocketName);
		FRotator SpawnRotation = GetActorRotation();

		ABaseVehicleSeat* Seat = GetWorld()->SpawnActor<ABaseVehicleSeat>(SeatClass, SpawnLocation, SpawnRotation, Parameters);
		Seat->AttachToComponent(VehicleMesh, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), SocketName);
		Seats.Add(Seat);

		FString Key = It.Key().ToString();
		FString Value = It.Value().Get()->GetDefaultObjectName().ToString();

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("SeatMap: (%s, %s)"), *Key, *Value));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Seats: %d"), Seats.Num()));
}

bool ABaseVehiclePawn::EnterVehicle(APawn* Pawn)
{
	ABaseVehicleSeat* Seat = GetClosestSeat(Pawn->GetActorLocation());

	return EnterVehicleSeat(Pawn, Seat);

}

bool ABaseVehiclePawn::EnterVehicleSeat(APawn* Pawn, ABaseVehicleSeat* Seat)
{
	if (Seat == nullptr || Seat->IsOccupied()) {

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Invalid Seat"));
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Enter"));
	Seat->EnterSeat(Pawn);
	return true;
}

ABaseVehicleSeat* ABaseVehiclePawn::GetClosestSeat(const FVector Position)
{
	float ClosestDistance = -1;
	ABaseVehicleSeat* ClosestSeat = nullptr;
	for (auto& Seat : Seats)
	{
		if (Seat)
		{
			float Distance = FVector::Distance(Position, Seat->GetActorLocation());
			if (Distance < ClosestDistance || ClosestDistance == -1) {
				ClosestSeat = Seat;
				ClosestDistance = Distance;
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Missing Seat"));
		}
	}
	return ClosestSeat;
}

AUnitAIController* ABaseVehiclePawn::GetUnitController()
{
	return Cast<AUnitAIController>(GetController());
}

FVector ABaseVehiclePawn::GetLocation_Implementation()
{
	return GetActorLocation();
}

UObject* ABaseVehiclePawn::GetCurrentOrder_Implementation()
{
	return GetUnitController()->GetCurrentOrder();
}

void ABaseVehiclePawn::StopCurrentOrder_Implementation()
{
	GetUnitController()->StopCurrentOrder();
	SetOrderIsComplete_Implementation(true);
}

bool ABaseVehiclePawn::IsIdle_Implementation()
{
	return GetUnitController()->IsIdle();
}

bool ABaseVehiclePawn::GetOrderIsComplete_Implementation()
{
	return HasCompletedOrder;
}

void ABaseVehiclePawn::SetOrderIsComplete_Implementation(bool IsComplete)
{
	HasCompletedOrder = IsComplete;
}

UObject* ABaseVehiclePawn::GetParentGroup_Implementation()
{
	return Cast<UObject>(ParentGroup);
}

void ABaseVehiclePawn::SetParentGroup_Implementation(UObject* Parent)
{
	ParentGroup = Cast<UUnitGroup>(Parent);
}
