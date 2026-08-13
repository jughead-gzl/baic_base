#include <atomic>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <thread>

#include "../structure/base/timer.h"

namespace
{
using Timer = structure::Timer;

void TestConfigureByPeriodAndFrequency()
{
    Timer timer(std::chrono::milliseconds(20));

    // Verify period configured by constructor.
    assert(timer.Period() == std::chrono::milliseconds(20));

    timer.SetFrequency(100.0);
    // Verify frequency setter updates period to 10ms.
    assert(timer.Period() == std::chrono::milliseconds(10));

    const double hz = timer.FrequencyHz();
    // Verify inverse conversion is close to requested frequency.
    assert(hz > 99.0 && hz < 101.0);
}

void TestWaitNextCycleAndStop()
{
    Timer timer(std::chrono::milliseconds(200));
    timer.Start();

    std::atomic<bool> wait_result{true};
    std::thread waiter([&]() {
        wait_result.store(timer.WaitNextCycle(), std::memory_order_relaxed);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    timer.Stop();

    waiter.join();
    // Verify wait exits with false when stopped before deadline.
    assert(!wait_result.load(std::memory_order_relaxed));
}

void TestPeriodicCadence()
{
    Timer timer(std::chrono::milliseconds(10));
    timer.Start();

    const auto t0 = std::chrono::steady_clock::now();
    assert(timer.WaitNextCycle());
    assert(timer.WaitNextCycle());
    assert(timer.WaitNextCycle());
    const auto t1 = std::chrono::steady_clock::now();

    timer.Stop();

    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    // Verify three cycles consume roughly at least two periods.
    assert(elapsed >= 20);
}

void TestOverrunTracking()
{
    Timer timer(std::chrono::milliseconds(2));
    timer.Start();

    assert(timer.WaitNextCycle());
    std::this_thread::sleep_for(std::chrono::milliseconds(9));
    assert(timer.WaitNextCycle());

    timer.Stop();

    // Verify missed cycles are tracked as overruns.
    assert(timer.OverrunCycles() > 0);

    timer.ResetOverrunCycles();
    // Verify reset API clears overrun count.
    assert(timer.OverrunCycles() == 0);
}
} // namespace

int main()
{
    TestConfigureByPeriodAndFrequency();
    TestWaitNextCycleAndStop();
    TestPeriodicCadence();
    TestOverrunTracking();
    return 0;
}
