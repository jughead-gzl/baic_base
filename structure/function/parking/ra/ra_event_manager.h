#pragma once

#include "ra_event_enum.h"
#include "../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/** @brief 保存和查询 RA 事件。 */
class RaEventManager : public ParkingEventManager<RaActvType, RaActvIhbtType, RaGuidanceType,
                                         RaGuidanceIhbtType, RaPauseType, RaSuccessType,
                                         RaFailType, RaExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
