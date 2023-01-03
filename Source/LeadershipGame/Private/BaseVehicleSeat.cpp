// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseVehicleSeat.h"

ABaseVehicleSeat::ABaseVehicleSeat()
{
	CurrentPawn = nullptr;
}

bool ABaseVehicleSeat::IsOccupied()
{
	return CurrentPawn != nullptr;
}

void ABaseVehicleSeat::EnterSeat(APawn* Pawn)
{
	if (IsOccupied()) return;

	CurrentPawn = Pawn;

	// Possess this seat
	Pawn->GetController()->Possess(this);

	Pawn->Destroy();
}

void ABaseVehicleSeat::LeaveSeat()
{
	if (!IsOccupied()) return;

	/*FActorSpawnParameters Parameters;
	Parameters.Template = CurrentPawn;
	APawn* Pawn = GetWorld()->SpawnActor<APawn>(CurrentPawn->GetClass(), Parameters);
	CurrentPawn = nullptr;*/

	// TODO: Possess Pawn
}