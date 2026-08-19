#pragma once
#include "apa_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/**
 * @brief 保存和查询 APA 事件。
 */
class ApaEventManager : public ParkingEventManager<ApaActvType, ApaActvIhbtType, ApaGuidanceType,
                                         ApaGuidanceIhbtType, ApaPauseType, ApaSuccessType,
                                         ApaFailType, ApaExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
