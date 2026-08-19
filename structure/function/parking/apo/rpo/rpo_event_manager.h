#pragma once

#include "rpo_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/** @brief RPO 事件类型。 */
/** @brief 保存和查询 RPO 事件。 */
class RpoEventManager : public ParkingEventManager<RpoActvType, RpoActvIhbtType, RpoGuidanceType,
                                         RpoGuidanceIhbtType, RpoPauseType, RpoSuccessType,
                                         RpoFailType, RpoExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
