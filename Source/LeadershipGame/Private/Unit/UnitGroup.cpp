// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/UnitGroup.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSFollowOrder.h"

UUnitGroup::UUnitGroup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Dispersion = 200.0f;
}

void UUnitGroup::Tick(float DeltaTime)
{
	/*for (auto& UnitPawn : Units) {
		IUnitInterface* Unit = Cast<IUnitInterface>(UnitPawn);
		UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (Unit->IsIdle_Implementation() ? TEXT("true") : TEXT("false")));
	}*/

	// If none of our units are busy
	if (Units.ContainsByPredicate([](APawn* Pawn) {
		IUnitInterface* Unit = Cast<IUnitInterface>(Pawn);
		return Unit->IsIdle_Implementation();
	})) {
		// And there are orders in the que
		if (!OrderQue.IsEmpty()) {
			// Send the next order
			SendNextOrder();
		}
	}
}

void UUnitGroup::IssueOrder(URTSOrder* Order)
{
	UE_LOG(LogTemp, Warning, TEXT("IssueOrder"));
	OrderQue.Enqueue(Order);
}

void UUnitGroup::SendNextOrder()
{
	// Stop if the que is empty
	if (OrderQue.IsEmpty()) {
		return;
	}

	// Retrive then remove order from que
	URTSOrder* Order = *OrderQue.Peek();
	UE_LOG(LogTemp, Warning, TEXT("SendNextOrder"));

	FUnitGroupFormation Formation = Order->Formation;
	if(Formation.HasInstructions()) {
		for (int n = 0; n < Units.Num(); n++)
		{
			APawn* Unit = Units[n];
			FMovementInstruction Instruction = Formation.Instructions.IsValidIndex(n) ? Formation.Instructions[n] : FMovementInstruction();

			// If this instruction is not the center of the formation: Give FollowOrder
			if (!Instruction.IsCenter()) {
				if (Units.IsValidIndex(Instruction.Index)) {
					// Give follow order
					URTSFollowOrder* FollowOrder = NewObject<URTSFollowOrder>();
					FollowOrder->TargetActor = Units[Instruction.Index];
					FollowOrder->Offset = Instruction.Offset * Dispersion;
					FollowOrder->IssueOrder(Unit);
					UE_LOG(LogTemp, Warning, TEXT("%d Following %d"), n, Instruction.Index);
					continue;
				}
			}
			// Otherwise: Give MoveOrder
			Order->IssueOrder(Unit);
		}
	}
	else {
		for (auto& Unit : Units) {
			Order->IssueOrder(Unit);
		}
	}

	OrderQue.Pop();
}

void UUnitGroup::StopAllOrders(URTSOrder* Order)
{
	OrderQue.Empty();
	StopCurrentOrder();
}

void UUnitGroup::StopCurrentOrder_Implementation()
{
	for (auto& UnitPawn : Units) {
		IUnitInterface* Unit = Cast<IUnitInterface>(UnitPawn);
		Unit->StopCurrentOrder();
	}
}

FVector UUnitGroup::GetLocation_Implementation()
{
	if (Units.IsEmpty()) {
		return FVector();
	}

	APawn* Leader = Units[0];
	return Leader->GetActorLocation();
}

TStatId UUnitGroup::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UUnitGroup, STATGROUP_Tickables);
}