#pragma once

#include "CoreMinimal.h"

#include "Orders/RTSOrder.h"

#include "RTSFollowOrder.generated.h"


/** Orders the actor to Follow to the specified location. */
UCLASS()
class LEADERSHIPGAME_API URTSFollowOrder : public URTSOrder
{
    GENERATED_BODY()

public:
    URTSFollowOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Target actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    AActor* TargetActor;

    /** Offset from target actor. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    FVector Offset;
};
