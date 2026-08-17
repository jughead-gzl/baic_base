#pragma once

#include <atomic>
#include <cstdint>

namespace structure
{

/**
 * @brief 生成进程内全局唯一的递增 ID。
 *
 * 所有 UniqueID 实例共享同一个原子计数器，适用于多线程环境。
 */
class UniqueID
{
public:
    using ValueType = std::uint64_t;

    /**
     * @brief 获取下一个全局唯一 ID。
     * @return 从 1 开始递增的 ID；计数器溢出时回绕到 0。
     */
    static ValueType Next() noexcept
    {
        return next_id_.fetch_add(1, std::memory_order_relaxed);
    }

private:
    inline static std::atomic<ValueType> next_id_{1};
};

} // namespace structure