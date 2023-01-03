#include "Orders/RTSOrder.h"
#include "UnitAIController.h"
#include "GameFramework/Pawn.h"
#include "Orders/RTSOrderData.h"

bool URTSOrder::CanObeyOrder(const AActor* OrderedActor, int32 Index) const
{
    return true;
}

bool URTSOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    return true;
}

void URTSOrder::IssueOrder(TScriptInterface<IUnitInterface> OrderedUnit) const
{
    UObject* Unit = OrderedUnit.GetObject();

    if (!IsValid(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("URTSOrder::IssueOrder : TScriptInterface<IUnitInterface> has no object"));
        return;
    }

    UUnitGroup* OrderedGroup = Cast<UUnitGroup>(Unit);
    if (IsValid(OrderedGroup))
    {
        return IssueOrder(OrderedGroup);
    }

    AActor* OrderedActor = Cast<AActor>(Unit);
    if (IsValid(OrderedActor))
    {
        return IssueOrder(OrderedActor);
    }

   

    return IssueOrder(Unit);
}

void URTSOrder::IssueOrder(UUnitInterface* OrderedUnit) const
{
    if (!IsValid(OrderedUnit))
    {
        return;
    }

    UUnitGroup* OrderedGroup = Cast<UUnitGroup>(OrderedUnit);
    if (IsValid(OrderedGroup))
    {
        return IssueOrder(OrderedGroup);
    }

    AActor* OrderedActor = Cast<AActor>(OrderedUnit);
    if (IsValid(OrderedActor))
    {
        return IssueOrder(OrderedActor);
    }
}

void URTSOrder::IssueOrder(AActor* OrderedActor) const
{
    APawn* Pawn = Cast<APawn>(OrderedActor);

    if (!IsValid(Pawn))
    {
        return;
    }

    AUnitAIController* PawnController = Cast<AUnitAIController>(Pawn->GetController());

    if (!IsValid(PawnController))
    {
        return;
    }

    PawnController->IssueOrder(const_cast<URTSOrder*>(this));
}

void URTSOrder::IssueOrder(UUnitGroup* OrderedGroup) const
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("URTSOrder::IssueOrder for UUnitGroup"));
    for (auto& Unit : OrderedGroup->Units) {
        IssueOrder(Unit);
    }
}

ERTSOrderTargetType URTSOrder::GetTargetType() const
{
    return TargetType;
}

ERTSOrderGroupExecutionType URTSOrder::GetGroupExecutionType() const
{
    return GroupExecutionType;
}

FRTSOrderTagRequirements URTSOrder::GetIssueTagRequirements() const
{
    return IssueTagRequirements;
}

UTexture2D* URTSOrder::GetNormalIcon() const
{
    return NormalIcon;
}

UTexture2D* URTSOrder::GetHoveredIcon() const
{
    return HoveredIcon;
}

UTexture2D* URTSOrder::GetPressedIcon() const
{
    return PressedIcon;
}

UTexture2D* URTSOrder::GetDisabledIcon() const
{
    return DisabledIcon;
}

FText URTSOrder::GetName() const
{
    return Name;
}

FText URTSOrder::GetDescription(const AActor* OrderedActor, int32 Index) const
{
    return Description;
}
