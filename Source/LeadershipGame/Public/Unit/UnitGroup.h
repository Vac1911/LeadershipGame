// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "UnitInterface.h"
#include "UnitGroupFormation.h"
#include "UnitGroup.generated.h"

class URTSOrder;


/** Group of units */
UCLASS(BlueprintType)
class LEADERSHIPGAME_API UUnitGroup : public UObject, public IUnitInterface
{
    GENERATED_BODY()

public:
    UUnitGroup(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName Name;

    /** Pawns in group. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<APawn*> Units;

    /** Formation for units to move in */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FUnitGroupFormation Formation;

    void IssueOrder(URTSOrder* Order);

    /** Get location of the group */
    FVector GetLocation_Implementation() override;
};
