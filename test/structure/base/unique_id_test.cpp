#include <algorithm>
#include <cassert>
#include <cstdint>
#include <set>
#include <thread>
#include <vector>

#include "../../../structure/base/unique_id.h"

namespace
{
void TestIDsAreSharedAndIncreasing()
{
    const auto first = structure::UniqueID::Next();
    const auto second = structure::UniqueID::Next();

    assert(first > 0);
    assert(second == first + 1);
}

void TestIDsAreUniqueAcrossThreads()
{
    constexpr std::size_t thread_count = 8;
    constexpr std::size_t ids_per_thread = 1000;
    std::vector<std::uint64_t> ids;
    std::vector<std::thread> threads;
    ids.resize(thread_count * ids_per_thread);

    for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index)
    {
        threads.emplace_back([&, thread_index]()
        {
            const auto offset = thread_index * ids_per_thread;
            for (std::size_t index = 0; index < ids_per_thread; ++index)
            {
                ids[offset + index] = structure::UniqueID::Next();
            }
        });
    }

    for (auto& thread : threads)
    {
        thread.join();
    }

    std::set<std::uint64_t> unique_ids(ids.begin(), ids.end());
    assert(unique_ids.size() == ids.size());
}
} // namespace

int main()
{
    TestIDsAreSharedAndIncreasing();
    TestIDsAreUniqueAcrossThreads();
    return 0;
}