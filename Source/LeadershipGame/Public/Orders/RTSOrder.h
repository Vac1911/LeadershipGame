#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "Orders/RTSOrderGroupExecutionType.h"
#include "Orders/RTSOrderTagRequirements.h"
#include "Orders/RTSOrderTargetData.h"
#include "Orders/RTSOrderTargetType.h"
#include "Unit/UnitGroupFormation.h"
#include "UnitInterface.h"
#include "RTSOrder.generated.h"


class UTexture2D;
class UnitAIController;


/** Order that can be issued to an actor. */
UCLASS(BlueprintType, Blueprintable)
class LEADERSHIPGAME_API URTSOrder : public UObject
{
    GENERATED_BODY()

public:
    URTSOrder(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    /** Formation to move in. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    FUnitGroupFormation Formation;

    /** Has the Order been completed? */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    bool IsCompleted;

    /** Whether the specified actor can obey this order. */
    virtual bool CanObeyOrder(const AActor* OrderedActor, int32 Index) const;

    /** Whether the specified actor and/or location is a valid target for this order. */
    virtual bool IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const;

    /** Issues this order to the specified unit. */
    virtual void IssueOrder(TScriptInterface<IUnitInterface> OrderedUnit) const;

    /** Issues this order to the specified unit. */
    virtual void IssueOrder(UUnitInterface* OrderedUnit) const;

    /** Issues this order to the specified actor. */
    virtual void IssueOrder(AActor* OrderedActor) const;

    ///** Issues this order to the specified group. */
    //virtual void IssueOrder(UUnitGroup* OrderedGroup) const;

    /** Gets the target required by this order. */
    ERTSOrderTargetType GetTargetType() const;

    /** Gets how many and which of the selected actors the order should be issued to. */
    ERTSOrderGroupExecutionType GetGroupExecutionType() const;

    /** Gets the tag requirements that must be fulfilled to issue the order. */
    FRTSOrderTagRequirements GetIssueTagRequirements() const;

    /** Gets the normal icon of the order. */
    UTexture2D* GetNormalIcon() const;

    /** Gets the hovered icon of the order. */
    UTexture2D* GetHoveredIcon() const;

    /** Gets the pressed icon of the order. */
    UTexture2D* GetPressedIcon() const;

    /** Gets the disabled icon of the order. */
    UTexture2D* GetDisabledIcon() const;

    /** Gets the name of the order. */
    FText GetName() const;

    /** Gets the description of the order. */
    virtual FText GetDescription(const AActor* OrderedActor, int32 Index) const;

protected:
    /** Target required by this order. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    ERTSOrderTargetType TargetType;

    /** How many and which of the selected actors the order should be issued to. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    ERTSOrderGroupExecutionType GroupExecutionType;

    /** Tag requirements that must be fulfilled to issue the order. */
    /*UPROPERTY(EditDefaultsOnly, Category = "RTS")*/
    FRTSOrderTagRequirements IssueTagRequirements;

    /** Tag requirements that must be fulfilled for the order to be successful. */
    /*UPROPERTY(EditDefaultsOnly, Category = "RTS")*/
    FRTSOrderTagRequirements SuccessTagRequirements;

    /** Normal icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* NormalIcon;

    /** Hovered icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* HoveredIcon;

    /** Pressed icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* PressedIcon;

    /** Disabled icon of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    UTexture2D* DisabledIcon;

    /** Name of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FText Name;

    /** Default description of the order. Can be shown in the UI. */
    UPROPERTY(EditDefaultsOnly, Category = "RTS")
    FText Description;
};
