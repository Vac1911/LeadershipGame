// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "UnitInterface.h"
#include "UnitGroup.generated.h"


/** Group of units */
UCLASS(BlueprintType)
class LEADERSHIPGAME_API UUnitGroup : public UObject, public IUnitInterface
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName Name;

    /** Pawns in group. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<APawn*> Units;

    /** Get location of the group */
    FVector GetLocation_Implementation() override;
};
