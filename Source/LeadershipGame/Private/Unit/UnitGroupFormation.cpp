// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/UnitGroupFormation.h"


FMovementInstruction::FMovementInstruction()
{
	Index = -1;
}

FMovementInstruction::FMovementInstruction(int FollowIndex, FVector FollowOffset)
{
	Index = FollowIndex;
	Offset = FollowOffset;
}

bool FMovementInstruction::IsCenter()
{
	return Index == -1;
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

const FUnitGroupFormation FUnitGroupFormation::Diamond = FUnitGroupFormation(
	FName(TEXT("Diamond")),
	TArray<FMovementInstruction>({
		FMovementInstruction(),
		FMovementInstruction(1, FVector::BackwardVector + FVector::RightVector),
		FMovementInstruction(2, FVector::BackwardVector + FVector::LeftVector),
		FMovementInstruction(3, FVector::BackwardVector * 2)
	})
);

bool FUnitGroupFormation::HasInstructions()
{
	return !Instructions.IsEmpty();
}