#pragma once

#include "avm_event_enum.h"
#include "../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

/** @brief 保存和查询 AVM 事件。 */
class AvmEventManager : public ParkingEventManager<AvmActvType, AvmActvIhbtType, AvmGuidanceType,
                                         AvmGuidanceIhbtType, AvmPauseType, AvmSuccessType,
                                         AvmFailType, AvmExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
