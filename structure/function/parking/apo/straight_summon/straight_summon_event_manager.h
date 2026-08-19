#pragma once

#include "straight_summon_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/** @brief 保存和查询直线召唤事件。 */
class StraightSummonEventManager : public ParkingEventManager<StraightSummonActvType, StraightSummonActvIhbtType,
                                                     StraightSummonGuidanceType, StraightSummonGuidanceIhbtType,
                                                     StraightSummonPauseType, StraightSummonSuccessType,
                                                     StraightSummonFailType, StraightSummonExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
