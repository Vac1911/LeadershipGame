#pragma once

#include "CoreMinimal.h"
#include "Orders/RTSOrder.h"
#include "UnitInterface.h"
#include "RTSMountOrder.generated.h"


/** Orders the actor to get in vehicle. */
UCLASS()
class LEADERSHIPGAME_API URTSMountOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSMountOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Vehicle(s) to get in. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    TScriptInterface<IUnitInterface> TargetUnit;
};
