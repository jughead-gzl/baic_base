#include <cassert>
#include <chrono>
#include <thread>

#include "../../../structure/concurrent/RingBuffer.h"

namespace
{
void TestFifoAndCapacity()
{
    concurrent::RingBuffer<int, 2> queue;

    assert(queue.capacity() == 2);
    assert(queue.empty());
    assert(queue.try_push(1));
    assert(queue.try_push(2));
    assert(queue.full());
    assert(!queue.try_push(3));

    int value = 0;
    assert(queue.try_pop(value));
    assert(value == 1);
    assert(queue.try_pop(value));
    assert(value == 2);
    assert(queue.empty());
    assert(!queue.try_pop(value));
}

void TestTimedOperations()
{
    concurrent::RingBuffer<int, 1> queue;
    int value = 0;

    assert(!queue.try_pop_for(value, std::chrono::milliseconds(1)));
    assert(queue.try_push_for(7, std::chrono::milliseconds(1)));
    assert(!queue.try_push_for(8, std::chrono::milliseconds(1)));
    assert(queue.try_pop_for(value, std::chrono::milliseconds(1)));
    assert(value == 7);
}

void TestBlockingOperations()
{
    concurrent::RingBuffer<int, 1> queue;
    std::thread producer([&queue]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        queue.push(42);
    });

    int value = 0;
    queue.pop(value);
    producer.join();
    assert(value == 42);
}
} // namespace

int main()
{
    TestFifoAndCapacity();
    TestTimedOperations();
    TestBlockingOperations();
    return 0;
}