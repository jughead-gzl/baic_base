#include <cassert>
#include <stdexcept>
#include <vector>

#include "../../../structure/pool/thread_pool.h"

namespace
{
void TestSubmitAndResults()
{
    structure::pool::ThreadPool pool(4);
    std::vector<std::future<int>> results;
    results.reserve(64);

    for (int i = 0; i < 64; ++i)
    {
        results.emplace_back(pool.submit([](int value) { return value * value; }, i));
    }

    for (int i = 0; i < 64; ++i)
    {
        assert(results[static_cast<std::size_t>(i)].get() == i * i);
    }
}

void TestExceptionsReachFuture()
{
    structure::pool::ThreadPool pool(1);
    auto result = pool.submit([]() -> int {
        throw std::runtime_error("task failed");
    });

    bool caught = false;
    try
    {
        result.get();
    }
    catch (const std::runtime_error&)
    {
        caught = true;
    }
    assert(caught);

    assert(pool.submit([] { return 7; }).get() == 7);
}

void TestZeroWorkersRejected()
{
    bool caught = false;
    try
    {
        structure::pool::ThreadPool pool(0);
    }
    catch (const std::invalid_argument&)
    {
        caught = true;
    }
    assert(caught);
}
} // namespace

int main()
{
    TestSubmitAndResults();
    TestExceptionsReachFuture();
    TestZeroWorkersRejected();
    return 0;
}