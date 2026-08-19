#include <cassert>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include "../../../structure/function/parking/parking_event_manager.h"
#include "../../../structure/function/parking/apa/apa/apa_event_manager.h"
#include "../../../structure/function/parking/apa/rpa/rpa_event_manager.h"
#include "../../../structure/function/parking/apo/apo/apo_event_manager.h"
#include "../../../structure/function/parking/apo/gesture_summon/gesture_summon_event_manager.h"
#include "../../../structure/function/parking/apo/rpo/rpo_event_manager.h"
#include "../../../structure/function/parking/apo/straight_summon/straight_summon_event_manager.h"
#include "../../../structure/function/parking/avm/avm_event_manager.h"
#include "../../../structure/function/parking/avp/cruising/avp_cruising_event_manager.h"
#include "../../../structure/function/parking/avp/mapping/avp_mapping_event_manager.h"
#include "../../../structure/function/parking/ra/ra_event_manager.h"

namespace
{
using namespace structure::function::parking;

using EventManager = ApaEventManager;

void TestDefaultState()
{
    EventManager manager;

    assert(manager.GetActv() == ApaActvType::NONE_0);
    assert(manager.GetActvInhibited() == ApaActvIhbtType::NONE_0);
    assert(manager.GetGuidance() == ApaGuidanceType::NONE_0);
    assert(manager.GetGuidanceInhibited() == ApaGuidanceIhbtType::NONE_0);
    assert(manager.GetPause() == ApaPauseType::NONE_0);
    assert(manager.GetSuccess() == ApaSuccessType::NONE_0);
    assert(manager.GetFail() == ApaFailType::NONE_0);
    assert(manager.GetExit() == ApaExitType::NONE_0);

    assert(!manager.GetEventFlag(ParkingEventType::ACTV_1));
    assert(!manager.GetEventFlag(ParkingEventType::EXIT_8));
    assert(!manager.GetEventFlag(ParkingEventType::NONE_0));
}

void TestSettersAndEventFlags()
{
    EventManager manager;

    manager.SetActv(ApaActvType::BTN_1);
    manager.SetActvInhibited(ApaActvIhbtType::SYS_FAULT_2);
    manager.SetGuidance(ApaGuidanceType::ACTIVE_1);
    manager.SetGuidanceInhibited(ApaGuidanceIhbtType::SYS_FAULT_2);
    manager.SetPause(ApaPauseType::SYS_FAULT_2);
    manager.SetSuccess(ApaSuccessType::RESULT_SAVED_2);
    manager.SetFail(ApaFailType::SYS_FAULT_1);
    manager.SetExit(ApaExitType::SYS_FAULT_4);

    assert(manager.GetActv() == ApaActvType::BTN_1);
    assert(manager.GetActvInhibited() == ApaActvIhbtType::SYS_FAULT_2);
    assert(manager.GetGuidance() == ApaGuidanceType::ACTIVE_1);
    assert(manager.GetGuidanceInhibited() == ApaGuidanceIhbtType::SYS_FAULT_2);
    assert(manager.GetPause() == ApaPauseType::SYS_FAULT_2);
    assert(manager.GetSuccess() == ApaSuccessType::RESULT_SAVED_2);
    assert(manager.GetFail() == ApaFailType::SYS_FAULT_1);
    assert(manager.GetExit() == ApaExitType::SYS_FAULT_4);

    for (std::uint8_t value = 1; value <= 8; ++value)
    {
        assert(manager.GetEventFlag(static_cast<ParkingEventType>(value)));
    }
    assert(manager.GetEventType() == ParkingEventType::ACTV_1);
}

void TestVariantAccess()
{
    EventManager manager;
    manager.SetActv(ApaActvType::APP_3);
    manager.SetExit(ApaExitType::USER_REQ_3);

    const auto actv = manager.GetEventType(ParkingEventType::ACTV_1);
    const auto exit = manager.GetEventType(ParkingEventType::EXIT_8);
    assert(std::holds_alternative<ApaActvType>(actv));
    assert(std::holds_alternative<ApaExitType>(exit));
    assert(std::get<ApaActvType>(actv) == ApaActvType::APP_3);
    assert(std::get<ApaExitType>(exit) == ApaExitType::USER_REQ_3);
}

void TestVariantVisitorSetter()
{
    EventManager manager;

    manager.SetEvent(EventManager::EventValue{ApaPauseType::USER_REQ_1});
    assert(manager.GetPause() == ApaPauseType::USER_REQ_1);
    assert(manager.GetEventFlag(ParkingEventType::PAUSE_5));
    assert(manager.GetEventType() == ParkingEventType::PAUSE_5);

    manager.SetEvent(EventManager::EventValue{ApaExitType::VEH_COND_5});
    assert(manager.GetExit() == ApaExitType::VEH_COND_5);
    assert(manager.GetEventFlag(ParkingEventType::EXIT_8));
}

void TestDerivedEventLog()
{
    EventManager manager;
    std::ostringstream log_stream;
    std::streambuf* const original_buffer = std::clog.rdbuf(log_stream.rdbuf());

    manager.SetActv(ApaActvType::BTN_1);

    std::clog.rdbuf(original_buffer);
    assert(log_stream.str().find("ApaEventManager: ACTV = BTN[1]") != std::string::npos);
}

void TestReset()
{
    EventManager manager;
    manager.SetActv(ApaActvType::KEY_2);
    manager.SetSuccess(ApaSuccessType::RESULT_SAVED_2);
    manager.Reset();

    assert(manager.GetActv() == ApaActvType::NONE_0);
    assert(manager.GetSuccess() == ApaSuccessType::NONE_0);
    assert(!manager.GetEventFlag(ParkingEventType::ACTV_1));
    assert(!manager.GetEventFlag(ParkingEventType::SUCCESS_6));
    assert(manager.GetEventType() == ParkingEventType::NONE_0);
}

void TestDerivedManagersCanBeConstructed()
{
    ApaEventManager apa;
    RpaEventManager rpa;
    ApoEventManager apo;
    GestureSummonEventManager gesture_summon;
    RpoEventManager rpo;
    StraightSummonEventManager straight_summon;
    AvmEventManager avm;
    AvpCruisingEventManager avp_cruising;
    AvpMappingEventManager avp_mapping;
    RaEventManager ra;

    static_cast<void>(apa);
    static_cast<void>(rpa);
    static_cast<void>(apo);
    static_cast<void>(gesture_summon);
    static_cast<void>(rpo);
    static_cast<void>(straight_summon);
    static_cast<void>(avm);
    static_cast<void>(avp_cruising);
    static_cast<void>(avp_mapping);
    static_cast<void>(ra);
}
}

int main()
{
    TestDefaultState();
    TestSettersAndEventFlags();
    TestVariantAccess();
    TestVariantVisitorSetter();
    TestDerivedEventLog();
    TestReset();
    TestDerivedManagersCanBeConstructed();
    return 0;
}
