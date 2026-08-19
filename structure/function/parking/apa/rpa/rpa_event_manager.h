#pragma once

#include "rpa_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/**
 * @brief 保存和查询 RPA 事件。
 */
class RpaEventManager : public ParkingEventManager<RpaActvType, RpaActvIhbtType, RpaGuidanceType,
                                         RpaGuidanceIhbtType, RpaPauseType, RpaSuccessType,
                                         RpaFailType, RpaExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
