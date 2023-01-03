// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/UnitGroup.h"

FVector UUnitGroup::GetLocation_Implementation()
{
	if (Units.IsEmpty()) {
		return FVector();
	}

	APawn* Leader = Units[0];
	return Leader->GetActorLocation();
}
