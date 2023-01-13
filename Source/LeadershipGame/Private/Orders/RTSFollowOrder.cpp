#include "Orders/RTSFollowOrder.h"
#include "Orders/RTSOrderTargetType.h"


URTSFollowOrder::URTSFollowOrder(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TargetType = ERTSOrderTargetType::ORDERTARGET_Actor;
    GroupExecutionType = ERTSOrderGroupExecutionType::ORDERGROUPEXECUTION_All;

    /*IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Immobilized());
    IssueTagRequirements.SourceBlockedTags.AddTag(URTSGameplayTagLibrary::Status_Changing_Constructing());*/
}
