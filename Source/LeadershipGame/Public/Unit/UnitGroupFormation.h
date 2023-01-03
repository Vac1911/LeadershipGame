// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"

#include "UnitGroupFormation.generated.h"


USTRUCT(BlueprintType)
struct LEADERSHIPGAME_API FMovementInstruction
{
    GENERATED_BODY()

public:
    FMovementInstruction();
    FMovementInstruction(int FollowIndex, FVector FollowOffset);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Index;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector Offset;
};


/** Formation for units to move in */
USTRUCT(BlueprintType)
struct LEADERSHIPGAME_API FUnitGroupFormation
{
    GENERATED_BODY()

public:
    FUnitGroupFormation();

    FUnitGroupFormation(FName FormationName, TArray<FMovementInstruction> FormationInstructions);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FMovementInstruction> Instructions;

    static const FUnitGroupFormation Column;
};
