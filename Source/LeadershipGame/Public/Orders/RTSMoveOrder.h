#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSMoveOrder.generated.h"


/** Orders the actor to move to the specified location. */
UCLASS()
class LEADERSHIPGAME_API URTSMoveOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSMoveOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Target location. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    FVector TargetLocation;
};
