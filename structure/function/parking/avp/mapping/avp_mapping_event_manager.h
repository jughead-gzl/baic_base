#pragma once

#include "avp_mapping_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/**
 * @brief 保存和查询 AVP 建图事件。
 *
 * 建图激活使用枚举保存激活来源和激活抑制原因，打开和打开抑制使用
 * 布尔事件标志，其余事件使用专用条件枚举保存。该类不负责判定条件、
 * 执行建图或驱动外部状态机。
 */
class AvpMappingEventManager : public ParkingEventManager<AvpmActvType, AvpmActvIhbtType, AvpmGuidanceType,
                                                AvpmGuidanceIhbtType, AvpmPauseType, AvpmSuccessType,
                                                AvpmFailType, AvpmExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
