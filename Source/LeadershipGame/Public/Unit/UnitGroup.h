// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "UnitInterface.h"
#include "UnitGroupFormation.h"
#include "UnitGroup.generated.h"

class URTSOrder;


/** Group of units */
UCLASS(BlueprintType)
class LEADERSHIPGAME_API UUnitGroup : public UObject, public IUnitInterface, public FTickableGameObject
{
    GENERATED_BODY()

public:
    UUnitGroup(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FName Name;

    /** Distance between units in group */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Dispersion;

    /** Units in group */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<APawn*> Units;

    // Called every frame
    virtual void Tick(float DeltaTime);

    void IssueOrder(URTSOrder* Order);

    void SendNextOrder();

    void StopAllOrders(URTSOrder* Order);

    void StopCurrentOrder_Implementation() override;

    FVector GetLocation_Implementation() override;

protected:
    TQueue<URTSOrder*> OrderQue;

    TStatId GetStatId() const;
};
