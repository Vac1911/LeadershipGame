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
}

void AUnitAIController::FindTargetInAcquisitionRadius()
{
	//if (!IsValid(AttackComponent))
	//{
	//	return;
	//}

	//// Find nearby actors.
	//TArray<AActor*> ActorsToIgnore;
	//ActorsToIgnore.Add(GetPawn());
	//
	//TArray<AActor*> NearbyActors;
	//UKismetSystemLibrary::SphereOverlapActors(this, GetPawn()->GetActorLocation(),
	//	AttackComponent->GetAcquisitionRadius(), AcquisitionObjectTypes, APawn::StaticClass(), ActorsToIgnore,
	//	NearbyActors);
	//
	//// Find target to acquire.
	//for (AActor* NearbyActor : NearbyActors)
	//{
	//	if (!IsValid(NearbyActor))
	//	{
	//		continue;
	//	}

	//	// Check owner.
	//	const AActor* MyActor = GetPawn();

	//	if (IsValid(MyActor))
	//	{
	//		const URTSOwnerComponent* MyOwnerComponent = MyActor->FindComponentByClass<URTSOwnerComponent>();

	//		if (MyOwnerComponent && MyOwnerComponent->IsSameTeamAsActor(NearbyActor))
	//		{
	//			continue;
	//		}
	//	}

	//	// Check if found attackable actor.
	//	if (!URTSGameplayTagLibrary::HasGameplayTag(NearbyActor, URTSGameplayTagLibrary::Status_Permanent_CanBeAttacked()))
	//	{
	//		continue;
	//	}

	//	// Acquire target.
	//	Blackboard->SetValueAsObject(TEXT("TargetActor"), NearbyActor);

	//	UE_LOG(LogTemp, Log, TEXT("%s automatically acquired target %s."), *GetPawn()->GetName(), *NearbyActor->GetName());
	//	return;
	//}
}

TSubclassOf<URTSOrder> AUnitAIController::GetCurrentOrder() const
{
	return Blackboard->GetValueAsClass(TEXT("OrderClass"));
}

bool AUnitAIController::HasOrder(ERTSOrderType OrderType) const
{
    UE_LOG(LogTemp, Warning, TEXT("AUnitAIController::HasOrder has been deprecated as of plugin version 1.2. Please use HasOrderByClass instead."));
    return Blackboard->GetValueAsEnum(TEXT("OrderType")) == (uint8)OrderType;
}

bool AUnitAIController::HasOrderByClass(TSubclassOf<URTSOrder> OrderClass) const
{
    return GetCurrentOrder() == OrderClass;
}

bool AUnitAIController::IsIdle() const
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

	Blackboard->SetValueAsObject(TEXT("CurrentOrder"), Order);

    // Update behavior tree.
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BehaviorTreeComponent)
    {
        BehaviorTreeComponent->RestartTree();
    }

    //// Apply order logic.
    //UCustomBlueprintLibrary::IssueOrder(GetOwner(), Order);

    //// Notify listeners.
    //OnCurrentOrderChanged.Broadcast(GetOwner(), Order);
}

void AUnitAIController::IssueAttackOrder(AActor* Target)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSAttackOrder::StaticClass();
    Order.TargetActor = Target;

    //IssueOrder(Order);
}

void AUnitAIController::IssueMoveOrder(const FVector& Location)
{
    FRTSOrderData Order;
    Order.OrderClass = URTSMoveOrder::StaticClass();
    Order.TargetLocation = Location;
    //IssueOrder(Order);
}

void AUnitAIController::IssueStopOrder()
{
    FRTSOrderData Order;
    Order.OrderClass = URTSStopOrder::StaticClass();
    //IssueOrder(Order);
}

ERTSOrderType AUnitAIController::OrderClassToType(UClass* OrderClass) const
{
    if (OrderClass == URTSAttackOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_Attack;
    }
    else if (OrderClass == URTSMoveOrder::StaticClass())
    {
        return ERTSOrderType::ORDER_Move;
    }

    return ERTSOrderType::ORDER_None;
}
