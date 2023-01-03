// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/UnitGroupFormation.h"


FMovementInstruction::FMovementInstruction()
{
}

FMovementInstruction::FMovementInstruction(int FollowIndex, FVector FollowOffset)
{
	Index = FollowIndex;
	Offset = FollowOffset;
}

FUnitGroupFormation::FUnitGroupFormation()
{
}

FUnitGroupFormation::FUnitGroupFormation(FName FormationName, TArray<FMovementInstruction> FormationInstructions)
{
	Name = FormationName;
	Instructions = FormationInstructions;
}

const FUnitGroupFormation FUnitGroupFormation::Column = FUnitGroupFormation(
	FName(TEXT("Column")),
	TArray<FMovementInstruction>({
		FMovementInstruction(),
		FMovementInstruction(1, FVector::BackwardVector),
		FMovementInstruction(2, FVector::BackwardVector),
		FMovementInstruction(3, FVector::BackwardVector)
	})
);