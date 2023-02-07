#include "UnitAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Orders/RTSAttackOrder.h"
#include "Orders/RTSMoveOrder.h"
#include "Orders/RTSStopOrder.h"


void AUnitAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

	//AttackComponent = InPawn->FindComponentByClass<URTSAttackComponent>();

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;

	if (UseBlackboard(PawnBlackboardAsset, BlackboardComponent))
	{
		// Setup blackboard.
		IssueStopOrder();
	}

    // Run behavior tree.
    RunBehaviorTree(PawnBehaviorTreeAsset);

	NavComponent = InPawn->FindComponentByClass<UMercunaGroundNavigationComponent>();
}

URTSOrder* AUnitAIController::GetCurrentOrder()
{
    UObject* OrderObj = Blackboard->GetValueAsObject(FName(TEXT("CurrentOrder")));
	return Cast<URTSOrder>(OrderObj);
}

bool AUnitAIController::HasOrderByClass(TSubclassOf<URTSOrder> OrderClass)
{
    URTSOrder* CurrentOrder = GetCurrentOrder();
    if (!CurrentOrder) {
        return false;
    }
    return GetCurrentOrder()->GetClass()->IsChildOf(OrderClass);
}

bool AUnitAIController::IsIdle()
{
    return HasOrderByClass(URTSStopOrder::StaticClass());
}

void AUnitAIController::IssueOrder(URTSOrder* Order)
{
    if (!Blackboard)
    {
        UE_LOG(LogTemp, Warning, TEXT("Blackboard not set up for %s, can't receive orders. Check AI Controller Class and Auto Possess AI."), *GetPawn()->GetName());
        return;
    }

	if (!Order)
	{
		UE_LOG(LogTemp, Error, TEXT("AUnitAIController::IssueOrder recieved nullptr as order"));
		return;
	}

    // Update blackboard.
	Blackboard->SetValueAsObject(FName(TEXT("CurrentOrder")), Order);

    // Update behavior tree.
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->RestartTree();
    }

    //// Notify listeners.
    //OnCurrentOrderChanged.Broadcast(GetOwner(), Order);
}

void AUnitAIController::IssueStopOrder()
{
    URTSStopOrder* StopOrder = NewObject<URTSStopOrder>();
    IssueOrder(StopOrder);
}

void AUnitAIController::CancelMovement()
{
	if (NavComponent) {
		NavComponent->CancelMovement(NavComponent->GetCurrentRequestId());
	}
}

void AUnitAIController::StopCurrentOrder()
{
	//Blackboard->SetValueAsObject(TEXT("CurrentOrder"), nullptr);
    IssueStopOrder();
}