#include <cassert>
#include <cstdint>
#include <limits>
#include <string>
#include <thread>
#include <vector>

#include "../structure/base/timestamp.h"

namespace
{
using TS = structure::Timestamp;

void TestConstructorsAndAssignment()
{
    TS t0;
    // Verify default constructor initializes to zero.
    assert(t0.Nanoseconds() == 0);

    TS t1(123456789LL);
    // Verify explicit nanoseconds constructor.
    assert(t1.Nanoseconds() == 123456789LL);

    TS t2(t1);
    // Verify copy constructor keeps value.
    assert(t2.Nanoseconds() == 123456789LL);

    TS t3;
    t3 = t1;
    // Verify copy assignment keeps value.
    assert(t3.Nanoseconds() == 123456789LL);

    TS t4(std::move(t1));
    // Verify move constructor keeps numeric value semantics.
    assert(t4.Nanoseconds() == 123456789LL);

    TS t5;
    t5 = std::move(t2);
    // Verify move assignment keeps numeric value semantics.
    assert(t5.Nanoseconds() == 123456789LL);
}

void TestFactoryAndUnitConversion()
{
    TS ns = TS::FromNanoseconds(1000);
    TS us = TS::FromMicroseconds(1);
    TS ms = TS::FromMilliseconds(1);
    TS sec = TS::FromSeconds(1);

    // Verify factory conversion values.
    assert(ns.Nanoseconds() == 1000);
    assert(us.Nanoseconds() == 1000);
    assert(ms.Nanoseconds() == 1000000);
    assert(sec.Nanoseconds() == 1000000000LL);

    // Verify read conversion accessors.
    assert(sec.Microseconds() == 1000000LL);
    assert(sec.Milliseconds() == 1000LL);
    assert(sec.Seconds() == 1LL);
}

void TestNowAPIs()
{
    TS s0 = TS::NowSystem();
    TS s1 = TS::NowSystem();
    // Verify system now is non-decreasing for immediate successive calls.
    assert(s1.Nanoseconds() >= s0.Nanoseconds());

    TS m0 = TS::NowSteady();
    TS m1 = TS::NowSteady();
    // Verify steady now is non-decreasing for immediate successive calls.
    assert(m1.Nanoseconds() >= m0.Nanoseconds());
}

void TestAtomicMutationAPIs()
{
    TS t(100);

    t.SetNanoseconds(200);
    // Verify atomic set.
    assert(t.Nanoseconds() == 200);

    std::int64_t old = t.ExchangeNanoseconds(300);
    // Verify exchange returns old value and sets new value.
    assert(old == 200);
    assert(t.Nanoseconds() == 300);

    std::int64_t add_ret = t.AddNanoseconds(25);
    // Verify add returns updated value.
    assert(add_ret == 325);
    assert(t.Nanoseconds() == 325);

    std::int64_t sub_ret = t.SubNanoseconds(5);
    // Verify subtract returns updated value.
    assert(sub_ret == 320);
    assert(t.Nanoseconds() == 320);
}

void TestArithmeticAndComparison()
{
    TS a(1000);
    TS b(1300);

    // Verify delta computation.
    assert(b.DeltaNanoseconds(a) == 300);
    assert(a.DeltaNanoseconds(b) == -300);

    TS plus = a.PlusNanoseconds(50);
    TS minus = b.MinusNanoseconds(100);
    // Verify non-mutating plus/minus.
    assert(plus.Nanoseconds() == 1050);
    assert(minus.Nanoseconds() == 1200);
    assert(a.Nanoseconds() == 1000);
    assert(b.Nanoseconds() == 1300);

    // Verify comparison operators.
    assert(a < b);
    assert(a <= b);
    assert(b > a);
    assert(b >= a);
    assert(a != b);
    assert(a == TS(1000));
}

void TestToString()
{
    TS t = TS::FromSeconds(1);
    std::string s = t.ToString();

    // Verify formatted string has date/time part and nanosecond suffix.
    assert(!s.empty());
    assert(s.find('.') != std::string::npos);
    assert(s.size() >= 19 + 1 + 9);
}

void TestMultiThreadAdd()
{
    TS t(0);
    constexpr int kThreadCount = 8;
    constexpr int kIterationsPerThread = 20000;

    std::vector<std::thread> threads;
    threads.reserve(kThreadCount);

    for (int i = 0; i < kThreadCount; ++i)
    {
        threads.emplace_back([&t]() {
            for (int j = 0; j < kIterationsPerThread; ++j)
            {
                t.AddNanoseconds(1);
            }
        });
    }

    for (auto& th : threads)
    {
        th.join();
    }

    // Verify atomic increments from multiple threads are not lost.
    assert(t.Nanoseconds() == static_cast<std::int64_t>(kThreadCount) * kIterationsPerThread);
}
} // namespace

int main()
{
    TestConstructorsAndAssignment();
    TestFactoryAndUnitConversion();
    TestNowAPIs();
    TestAtomicMutationAPIs();
    TestArithmeticAndComparison();
    TestToString();
    TestMultiThreadAdd();
    return 0;
}
