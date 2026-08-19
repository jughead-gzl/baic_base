#pragma once

#include "apo_event_enum.h"
#include "../../parking_event_manager.h"

namespace structure
{
namespace function
{
namespace parking
{

class ApoEventManager : public ParkingEventManager<ApoActvType, ApoActvIhbtType, ApoGuidanceType,
                                         ApoGuidanceIhbtType, ApoPauseType, ApoSuccessType,
                                         ApoFailType, ApoExitType>
{
};

}  // namespace parking
}  // namespace function
}  // namespace structure
