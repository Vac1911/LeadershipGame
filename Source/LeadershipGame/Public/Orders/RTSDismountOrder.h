#pragma once

#include "CoreMinimal.h"
#include "Orders/RTSMoveOrder.h"
#include "RTSDismountOrder.generated.h"


/** Orders the actor to get out of a vehicle. */
UCLASS()
class LEADERSHIPGAME_API URTSDismountOrder : public URTSMoveOrder
{
    GENERATED_BODY()

public:
    URTSDismountOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
