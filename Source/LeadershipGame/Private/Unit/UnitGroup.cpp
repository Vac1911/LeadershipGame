// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/UnitGroup.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSFollowOrder.h"

UUnitGroup::UUnitGroup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UUnitGroup::IssueOrder(URTSOrder* Order)
{
	if(Formation.HasInstructions()) {
		for (int n = 0; n < Units.Num(); n++)
		{
			APawn* Unit = Units[n];
			FMovementInstruction Instruction = Formation.Instructions.IsValidIndex(n) ? Formation.Instructions[n] : FMovementInstruction();
			if (!Instruction.IsCenter()) {
				if (Units.IsValidIndex(Instruction.Index)) {
					// Give follow order
					URTSFollowOrder* FollowOrder = NewObject<URTSFollowOrder>();
					FollowOrder->TargetActor = Units[Instruction.Index];
					FollowOrder->Offset = Instruction.Offset;
					continue;
				}
			}
			// Give move order
			Order->IssueOrder(Unit);
		}
	}
	else {
		for (auto& Unit : Units) {
			Order->IssueOrder(Unit);
		}
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
