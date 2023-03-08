#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Templates/SubclassOf.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "Orders/RTSOrderType.h"
#include "Components/MercunaGroundNavigationComponent.h"
#include "UnitAIController.generated.h"


class URTSAttackComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnitAIControllerCurrentOrderChangedSignature, AActor*, Actor, FRTSOrderData&, NewOrder);


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS()
class LEADERSHIPGAME_API AUnitAIController : public AAIController
{
    GENERATED_BODY()

public:
	/** Gets the class of the order currently issued to the pawn. */
	UFUNCTION(BlueprintPure)
    URTSOrder* GetCurrentOrder();

    /** Checks whether the pawn has an order of the specified type. */
    UFUNCTION(BlueprintPure)
    bool HasOrderByClass(TSubclassOf<URTSOrder> OrderClass);

    /** Checks whether the pawn is idle, or has any orders. */
    UFUNCTION(BlueprintPure)
    bool IsIdle();

    /** Checks whether the current order is conmplete. */
    UFUNCTION(BlueprintPure)
    bool IsComplete();

    /** Makes the pawn carry out the specified order. */
    void IssueOrder(URTSOrder* Order);

	/** Makes the pawn stop all actions immediately. */
	void IssueStopOrder();

    /** Event when the pawn has received a new order. */
    UPROPERTY(BlueprintAssignable, Category = "RTS")
    FUnitAIControllerCurrentOrderChangedSignature OnCurrentOrderChanged;

    /** Terminate any active movement actions */
    UFUNCTION(BlueprintCallable)
    void CancelMovement();

    /** Remove Current Order */
    UFUNCTION(BlueprintCallable)
    void StopCurrentOrder();

    /** Mark Current Order as Completed */
    UFUNCTION(BlueprintCallable)
    void CompleteCurrentOrder();

protected:
	virtual void OnPossess(APawn* InPawn) override;

    UMercunaNavigationComponent* NavComponent;

private:
    /** Behavior tree to use for driving the pawn AI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UBehaviorTree* PawnBehaviorTreeAsset;

    /** Blackboard to use for holding all data relevant to the pawn AI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UBlackboardData* PawnBlackboardAsset;

	/** Types of actors to filter for when trying to find targets in the acquisition radius. */
	UPROPERTY(EditDefaultsOnly, Category = "RTS")
	TArray<TEnumAsByte<EObjectTypeQuery>> AcquisitionObjectTypes;
};
