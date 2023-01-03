#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Templates/SubclassOf.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "Orders/RTSOrderType.h"
#include "CustomBlueprintLibrary.h"
#include "UnitAIController.generated.h"


class URTSAttackComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUnitAIControllerCurrentOrderChangedSignature, AActor*, Actor, const FRTSOrderData&, NewOrder);


/**
* AI controller that drives RTS unit movement and orders.
*/
UCLASS()
class LEADERSHIPGAME_API AUnitAIController : public AAIController
{
    GENERATED_BODY()

public:
	/** Makes the pawn look for a feasible target in its acquisition radius. */
	UFUNCTION(BlueprintCallable)
	void FindTargetInAcquisitionRadius();

	/** Gets the class of the order currently issued to the pawn. */
	UFUNCTION(BlueprintPure)
	TSubclassOf<URTSOrder> GetCurrentOrder() const;
	
    /** Checks whether the pawn has an order of the specified type. */
    UFUNCTION(BlueprintPure)
    bool HasOrder(ERTSOrderType OrderType) const;

    /** Checks whether the pawn has an order of the specified type. */
    UFUNCTION(BlueprintPure)
    bool HasOrderByClass(TSubclassOf<URTSOrder> OrderClass) const;

    /** Checks whether the pawn is idle, or has any orders. */
    UFUNCTION(BlueprintPure)
    bool IsIdle() const;

    /** Makes the pawn carry out the specified order. */
    void IssueOrder(URTSOrder* Order);

	/** Makes the pawn attack the specified target. */
	void IssueAttackOrder(AActor* Target);

    /** Makes the pawn move towards the specified location. */
    void IssueMoveOrder(const FVector& Location);

	/** Makes the pawn stop all actions immediately. */
	void IssueStopOrder();

    /** Event when the pawn has received a new order. */
    UPROPERTY(BlueprintAssignable, Category = "RTS")
    FUnitAIControllerCurrentOrderChangedSignature OnCurrentOrderChanged;

protected:
	virtual void OnPossess(APawn* InPawn) override;

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
	
    /*UPROPERTY()
	URTSAttackComponent* AttackComponent;*/

    ERTSOrderType OrderClassToType(UClass* OrderClass) const;
};
