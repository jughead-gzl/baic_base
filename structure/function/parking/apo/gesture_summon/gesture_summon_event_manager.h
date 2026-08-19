 #pragma once

#include "gesture_summon_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/** @brief 保存和查询手势召唤事件。 */
class GestureSummonEventManager : public ParkingEventManager<GestureSummonActvType, GestureSummonActvIhbtType,
												   GestureSummonGuidanceType, GestureSummonGuidanceIhbtType,
												   GestureSummonPauseType, GestureSummonSuccessType,
												   GestureSummonFailType, GestureSummonExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
