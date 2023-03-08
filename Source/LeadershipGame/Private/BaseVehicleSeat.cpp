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
	Vehicle->AttachToActor(CurrentPawn, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, false), SocketName);
}

void ABaseVehicleSeat::LeaveSeat()
{
	if (!IsOccupied()) return;


	CurrentPawn->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	CurrentPawn = nullptr;
}