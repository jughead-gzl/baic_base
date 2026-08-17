#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace structure
{

/**
 * @brief Thread-safe timestamp stored as nanoseconds since an epoch.
 *
 * Design goals:
 * - Fast lock-free reads/writes for multi-threaded producers and consumers.
 * - Compact representation based on signed 64-bit nanoseconds.
 * - Utility helpers for construction, conversion, arithmetic, comparison,
 *   and string formatting.
 *
 * Thread-safety model:
 * - Internal storage uses std::atomic<std::int64_t>.
 * - All state access/modification APIs are safe to call concurrently.
 * - memory_order_relaxed is intentionally used because the class provides
 *   atomic numeric state, not cross-object synchronization semantics.
 */
class Timestamp
{
private:
    /** @brief Number of nanoseconds in one microsecond. */
    static constexpr std::int64_t kNanosPerMicrosecond = 1000LL;
    /** @brief Number of nanoseconds in one millisecond. */
    static constexpr std::int64_t kNanosPerMillisecond = 1000LL * kNanosPerMicrosecond;
    /** @brief Number of nanoseconds in one second. */
    static constexpr std::int64_t kNanosPerSecond = 1000LL * kNanosPerMillisecond;

    /** @brief Atomic timestamp value in nanoseconds. */
    std::atomic<std::int64_t> nanoseconds_{0};

public:
    /**
     * @brief Construct a zero timestamp.
     *
     * The default value is 0 nanoseconds.
     */
    Timestamp() noexcept = default;

    /**
     * @brief Construct from raw nanoseconds.
     * @param nanoseconds Timestamp value in nanoseconds.
     */
    explicit Timestamp(std::int64_t nanoseconds) noexcept : nanoseconds_(nanoseconds)
    {
    }

    /**
     * @brief Copy constructor.
     * @param other Source timestamp.
     *
     * Copies the current atomic value from other.
     */
    Timestamp(const Timestamp& other) noexcept : nanoseconds_(other.Nanoseconds())
    {
    }

    /**
     * @brief Copy assignment.
     * @param other Source timestamp.
     * @return Reference to this object.
     */
    Timestamp& operator=(const Timestamp& other) noexcept
    {
        if (this != &other)
        {
            nanoseconds_.store(other.Nanoseconds(), std::memory_order_relaxed);
        }
        return *this;
    }

    /**
     * @brief Move constructor.
     * @param other Source timestamp.
     *
     * Semantically equivalent to copying the numeric value.
     */
    Timestamp(Timestamp&& other) noexcept : nanoseconds_(other.Nanoseconds())
    {
    }

    /**
     * @brief Move assignment.
     * @param other Source timestamp.
     * @return Reference to this object.
     *
     * Semantically equivalent to copying the numeric value.
     */
    Timestamp& operator=(Timestamp&& other) noexcept
    {
        if (this != &other)
        {
            nanoseconds_.store(other.Nanoseconds(), std::memory_order_relaxed);
        }
        return *this;
    }

    ~Timestamp() = default;

public:
    /**
     * @brief Create a timestamp from nanoseconds.
     * @param value Nanoseconds value.
     */
    static Timestamp FromNanoseconds(std::int64_t value) noexcept
    {
        return Timestamp(value);
    }

    /**
     * @brief Create a timestamp from microseconds.
     * @param value Microseconds value.
     */
    static Timestamp FromMicroseconds(std::int64_t value) noexcept
    {
        return Timestamp(value * kNanosPerMicrosecond);
    }

    /**
     * @brief Create a timestamp from milliseconds.
     * @param value Milliseconds value.
     */
    static Timestamp FromMilliseconds(std::int64_t value) noexcept
    {
        return Timestamp(value * kNanosPerMillisecond);
    }

    /**
     * @brief Create a timestamp from seconds.
     * @param value Seconds value.
     */
    static Timestamp FromSeconds(std::int64_t value) noexcept
    {
        return Timestamp(value * kNanosPerSecond);
    }

    /**
     * @brief Get current system-clock timestamp.
     * @return Timestamp based on std::chrono::system_clock.
     *
     * This is typically wall-clock time and may jump due to clock adjustments.
     */
    static Timestamp NowSystem() noexcept
    {
        const auto now = std::chrono::system_clock::now().time_since_epoch();
        const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
        return Timestamp(ns);
    }

    /**
     * @brief Get current steady-clock timestamp.
     * @return Timestamp based on std::chrono::steady_clock.
     *
     * This is monotonic in normal conditions and suitable for interval timing.
     */
    static Timestamp NowSteady() noexcept
    {
        const auto now = std::chrono::steady_clock::now().time_since_epoch();
        const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
        return Timestamp(ns);
    }

public:
    /**
     * @brief Read value in nanoseconds.
     * @return Current timestamp in nanoseconds.
     */
    std::int64_t Nanoseconds() const noexcept
    {
        return nanoseconds_.load(std::memory_order_relaxed);
    }

    /**
     * @brief Read value in microseconds.
     * @return Current timestamp in microseconds (integer truncation).
     */
    std::int64_t Microseconds() const noexcept
    {
        return Nanoseconds() / kNanosPerMicrosecond;
    }

    /**
     * @brief Read value in milliseconds.
     * @return Current timestamp in milliseconds (integer truncation).
     */
    std::int64_t Milliseconds() const noexcept
    {
        return Nanoseconds() / kNanosPerMillisecond;
    }

    /**
     * @brief Read value in seconds.
     * @return Current timestamp in seconds (integer truncation).
     */
    std::int64_t Seconds() const noexcept
    {
        return Nanoseconds() / kNanosPerSecond;
    }

    /**
     * @brief Atomically overwrite timestamp value.
     * @param value New nanoseconds value.
     */
    void SetNanoseconds(std::int64_t value) noexcept
    {
        nanoseconds_.store(value, std::memory_order_relaxed);
    }

    /**
     * @brief Atomically exchange timestamp value.
     * @param value New nanoseconds value.
     * @return Previous nanoseconds value.
     */
    std::int64_t ExchangeNanoseconds(std::int64_t value) noexcept
    {
        return nanoseconds_.exchange(value, std::memory_order_relaxed);
    }

    /**
     * @brief Atomically add nanoseconds.
     * @param delta Increment in nanoseconds.
     * @return New value after addition.
     */
    std::int64_t AddNanoseconds(std::int64_t delta) noexcept
    {
        return nanoseconds_.fetch_add(delta, std::memory_order_relaxed) + delta;
    }

    /**
     * @brief Atomically subtract nanoseconds.
     * @param delta Decrement in nanoseconds.
     * @return New value after subtraction.
     */
    std::int64_t SubNanoseconds(std::int64_t delta) noexcept
    {
        return nanoseconds_.fetch_sub(delta, std::memory_order_relaxed) - delta;
    }

public:
    /**
     * @brief Compute this - other in nanoseconds.
     * @param other Right-hand timestamp.
     * @return Signed difference in nanoseconds.
     */
    std::int64_t DeltaNanoseconds(const Timestamp& other) const noexcept
    {
        return Nanoseconds() - other.Nanoseconds();
    }

    /**
     * @brief Create a new timestamp offset by +delta.
     * @param delta Offset in nanoseconds.
     * @return New timestamp value.
     */
    Timestamp PlusNanoseconds(std::int64_t delta) const noexcept
    {
        return Timestamp(Nanoseconds() + delta);
    }

    /**
     * @brief Create a new timestamp offset by -delta.
     * @param delta Offset in nanoseconds.
     * @return New timestamp value.
     */
    Timestamp MinusNanoseconds(std::int64_t delta) const noexcept
    {
        return Timestamp(Nanoseconds() - delta);
    }

public:
    /** @brief Equality comparison by nanosecond value. */
    bool operator==(const Timestamp& other) const noexcept
    {
        return Nanoseconds() == other.Nanoseconds();
    }

    /** @brief Inequality comparison by nanosecond value. */
    bool operator!=(const Timestamp& other) const noexcept
    {
        return !(*this == other);
    }

    /** @brief Strict less-than comparison by nanosecond value. */
    bool operator<(const Timestamp& other) const noexcept
    {
        return Nanoseconds() < other.Nanoseconds();
    }

    /** @brief Less-than-or-equal comparison by nanosecond value. */
    bool operator<=(const Timestamp& other) const noexcept
    {
        return Nanoseconds() <= other.Nanoseconds();
    }

    /** @brief Strict greater-than comparison by nanosecond value. */
    bool operator>(const Timestamp& other) const noexcept
    {
        return Nanoseconds() > other.Nanoseconds();
    }

    /** @brief Greater-than-or-equal comparison by nanosecond value. */
    bool operator>=(const Timestamp& other) const noexcept
    {
        return Nanoseconds() >= other.Nanoseconds();
    }

public:
    /**
     * @brief Convert timestamp to local-time string.
     * @return String in format "YYYY-MM-DD HH:MM:SS.nnnnnnnnn".
     *
     * Notes:
     * - Uses local time zone conversion.
     * - For steady-clock derived values, this is numeric formatting only and
     *   does not represent wall-clock meaning.
     */
    std::string ToString() const
    {
        const std::int64_t ns_total = Nanoseconds();
        const std::time_t sec = static_cast<std::time_t>(ns_total / kNanosPerSecond);
        const std::int64_t ns_remainder = ns_total % kNanosPerSecond;

        std::tm tm_snapshot{};
#if defined(_WIN32)
        localtime_s(&tm_snapshot, &sec);
#else
        localtime_r(&sec, &tm_snapshot);
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm_snapshot, "%Y-%m-%d %H:%M:%S") << "."
            << std::setw(9) << std::setfill('0') << ns_remainder;
        return oss.str();
    }
};

};
