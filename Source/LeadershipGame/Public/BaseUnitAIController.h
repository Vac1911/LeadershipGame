#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Templates/SubclassOf.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "Orders/RTSOrderType.h"
#include "CustomBlueprintLibrary.h"
#include "BaseUnitAIController.generated.h"


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS()
class LEADERSHIPGAME_API ABaseUnitAIController : public AAIController
{
    GENERATED_BODY()

public:
	/** Gets the class of the order currently issued to the pawn. */
	/*UFUNCTION(BlueprintPure)
	TSubclassOf<URTSOrder> GetCurrentOrder() const;*/

    /** Makes the pawn carry out the specified order. */
    /*void IssueOrder(URTSOrder* Order);*/
};
