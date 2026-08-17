#pragma once

#include <atomic>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>

namespace structure
{

/**
 * @brief Thread period controller based on steady_clock.
 *
 * Typical usage in a worker thread:
 * @code
 * structure::Timer timer(std::chrono::milliseconds(20)); // 50 Hz
 * timer.Start();
 * while (running && timer.WaitNextCycle())
 * {
 *     // periodic task body
 * }
 * @endcode
 *
 * Design points:
 * - Uses steady_clock to avoid wall-clock jumps.
 * - Wait is interruptible by Stop().
 * - Supports runtime period/frequency update.
 * - Tracks cycle overruns when task execution exceeds period.
 */
class Timer
{
public:
    using clock = std::chrono::steady_clock;
    using duration = clock::duration;
    using nanoseconds = std::chrono::nanoseconds;

private:
    std::atomic<bool> running_{false};
    std::atomic<std::uint64_t> overrun_cycles_{0};

    mutable std::mutex mutex_;
    std::condition_variable cv_;
    duration period_{duration::zero()};
    clock::time_point next_deadline_{};
    bool started_{false};

public:

    Timer() = default;

    explicit Timer(duration period)
    {
        SetPeriod(period);
    }

    ~Timer()
    {
        Stop();
    }

    /**
     * @brief Start cycle timing.
     *
     * After start, each WaitNextCycle() returns once per period.
     */
    void Start()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        EnsurePeriodValidLocked();
        running_.store(true, std::memory_order_relaxed);
        overrun_cycles_.store(0, std::memory_order_relaxed);
        started_ = false;
        cv_.notify_all();
    }

    /**
     * @brief Stop waiting cycles.
     *
     * Any thread blocked in WaitNextCycle() will be woken up and return false.
     */
    void Stop() noexcept
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            running_.store(false, std::memory_order_relaxed);
            started_ = false;
        }
        cv_.notify_all();
    }

    /**
     * @brief Block until next cycle point.
     * @return true if cycle tick reached, false if timer stopped.
     */
    bool WaitNextCycle()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        EnsurePeriodValidLocked();

        if (!running_.load(std::memory_order_relaxed))
        {
            return false;
        }

        const auto now = clock::now();
        if (!started_)
        {
            next_deadline_ = now + period_;
            started_ = true;
        }

        // Catch up when current time already exceeds scheduled deadline.
        if (now >= next_deadline_)
        {
            const auto lag = now - next_deadline_;
            const auto missed = static_cast<std::uint64_t>(lag / period_) + 1ULL;
            if (missed > 1ULL)
            {
                overrun_cycles_.fetch_add(missed - 1ULL, std::memory_order_relaxed);
            }
            next_deadline_ += period_ * static_cast<duration::rep>(missed);
            return true;
        }

        const auto deadline = next_deadline_;
        const auto stopped = cv_.wait_until(lock, deadline, [this]() {
            return !running_.load(std::memory_order_relaxed);
        });
        if (stopped)
        {
            return false;
        }

        next_deadline_ += period_;
        return true;
    }

    /**
     * @brief Set period duration.
     * @param period Desired cycle period, must be positive.
     */
    void SetPeriod(duration period)
    {
        if (period <= duration::zero())
        {
            throw std::invalid_argument("period must be > 0");
        }

        std::lock_guard<std::mutex> lock(mutex_);
        period_ = period;
        started_ = false;
        cv_.notify_all();
    }

    /**
     * @brief Set frequency by Hz.
     * @param frequency_hz Desired frequency, must be finite and > 0.
     */
    void SetFrequency(double frequency_hz)
    {
        if (!(frequency_hz > 0.0) || !std::isfinite(frequency_hz))
        {
            throw std::invalid_argument("frequency_hz must be positive finite");
        }

        const auto period_ns = static_cast<std::int64_t>(1e9 / frequency_hz);
        if (period_ns <= 0)
        {
            throw std::invalid_argument("frequency_hz is too high");
        }
        SetPeriod(std::chrono::nanoseconds(period_ns));
    }

    /** @brief Get current period. */
    duration Period() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return period_;
    }

    /** @brief Get current configured frequency in Hz. */
    double FrequencyHz() const
    {
        const auto period = Period();
        const auto ns = std::chrono::duration_cast<nanoseconds>(period).count();
        return 1e9 / static_cast<double>(ns);
    }

    /** @brief Check whether timer is currently running. */
    bool IsRunning() const noexcept
    {
        return running_.load(std::memory_order_relaxed);
    }

    /** @brief Get accumulated overrun cycle count. */
    std::uint64_t OverrunCycles() const noexcept
    {
        return overrun_cycles_.load(std::memory_order_relaxed);
    }

    /** @brief Reset accumulated overrun cycle count to zero. */
    void ResetOverrunCycles() noexcept
    {
        overrun_cycles_.store(0, std::memory_order_relaxed);
    }

private:
    void EnsurePeriodValidLocked() const
    {
        if (period_ <= duration::zero())
        {
            throw std::logic_error("timer period is not configured");
        }
    }
};

} // namespace structure