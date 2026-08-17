#pragma once

#include "..\..\base\point6d.h"
#include "..\..\base\timestamp.h"

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace structure
{

/**
 * @brief Odometry state based on 6-DOF pose representation.
 *
 * The class stores timestamp, pose, twist and acceleration in 6-DOF form:
 * - pose: [x, y, z, roll, pitch, yaw]
 * - twist: first-order rates for the same 6 dimensions
 * - accel: second-order rates for the same 6 dimensions
 *
 * Coordinate-frame convention:
 * - frame_id: reference/world frame in which pose is expressed
 * - child_frame_id: body frame whose pose is being described
 */
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
class Odometry
{
public:
    /** @brief Scalar value type. */
    using value_type = T;
    /** @brief 6-DOF pose/twist/acceleration type. */
    using Pose = Point6D<T>;

private:
    /** @brief Timestamp for current odometry state. */
    Timestamp timestamp_{};
    /** @brief Pose of child_frame_id in frame_id. */
    Pose pose_{};
    /** @brief First-order derivative of pose components. */
    Pose twist_{};
    /** @brief Second-order derivative of pose components. */
    Pose accel_{};
    /** @brief Reference/world frame id (e.g. "map" or "odom"). */
    std::string frame_id_{"map"};
    /** @brief Body frame id (e.g. "base_link"). */
    std::string child_frame_id_{"base_link"};

public:
    /**
     * @brief Construct default odometry.
     *
     * Defaults:
     * - zero timestamp
     * - zero pose/twist/acceleration
     * - frame_id = "map"
     * - child_frame_id = "base_link"
     */
    Odometry() = default;

    /**
     * @brief Construct an Odometry object with specified parameters.
     * @param timestamp The timestamp of the odometry data.
     * @param pose The 6-DOF pose (position and orientation).
     * @param twist The 6-DOF twist (linear and angular velocities).
     * @param accel The 6-DOF acceleration (linear and angular accelerations).
     * @param frame_id The reference frame ID (default is "map").
     * @param child_frame_id The child frame ID (default is "base_link").
     */
    template <typename T1, typename T2, typename T3, 
    typename = typename std::enable_if_t<std::is_same_v<Timestamp, std::decay_t<T1>> && std::is_same_v<Pose, std::decay_t<T2>> && std::is_same_v<std::string, std::decay_t<T3>>>>
    Odometry(T1&& timestamp,
             T2&& pose,
             T2&& twist = Pose{},
             T2&& accel = Pose{},
             T3&& frame_id = "map",
             T3&& child_frame_id = "base_link")
        : timestamp_(std::forward<T1>(timestamp)),
          pose_(std::forward<T2>(pose)),
          twist_(std::forward<T2>(twist)),
          accel_(std::forward<T2>(accel)),
          frame_id_(std::forward<T3>(frame_id)),
          child_frame_id_(std::forward<T3>(child_frame_id))
    {
    }

    Odometry(const Odometry&) = default;
    Odometry& operator=(const Odometry&) = default;
    Odometry(Odometry&&) = default;
    Odometry& operator=(Odometry&&) = default;
    ~Odometry() = default;

public:
    /** @brief Get timestamp (const). */
    const Timestamp& GetTimestamp() const noexcept
    {
        return timestamp_;
    }

    /** @brief Get timestamp (mutable). */
    Timestamp& GetTimestamp() noexcept
    {
        return timestamp_;
    }

    /** @brief Get pose (const). */
    const Pose& GetPose() const noexcept
    {
        return pose_;
    }

    /** @brief Get pose (mutable). */
    Pose& GetPose() noexcept
    {
        return pose_;
    }

    /** @brief Get twist (const). */
    const Pose& GetTwist() const noexcept
    {
        return twist_;
    }

    /** @brief Get twist (mutable). */
    Pose& GetTwist() noexcept
    {
        return twist_;
    }

    /** @brief Get acceleration (const). */
    const Pose& GetAccel() const noexcept
    {
        return accel_;
    }

    /** @brief Get acceleration (mutable). */
    Pose& GetAccel() noexcept
    {
        return accel_;
    }

    /** @brief Get reference frame id. */
    const std::string& GetFrameId() const noexcept
    {
        return frame_id_;
    }

    /** @brief Get child/body frame id. */
    const std::string& GetChildFrameId() const noexcept
    {
        return child_frame_id_;
    }

public:
    /**
     * @brief Set the timestamp of the odometry data.
     * @tparam U Type of the timestamp, must be a Timestamp type.
     * @param timestamp The new timestamp to set.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Timestamp, std::decay_t<U>>>>
    void SetTimestamp(U&& timestamp) noexcept
    {
        timestamp_ = std::forward<U>(timestamp);
    }

    /**
     * @brief Set pose.
     * @tparam U Type of pose, must be Pose.
     * @param pose New pose value.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Pose, std::decay_t<U>>>>
    void SetPose(U&& pose) noexcept
    {
        pose_ = std::forward<U>(pose);
    }

    /**
     * @brief Set twist.
     * @tparam U Type of twist, must be Pose.
     * @param twist New twist value.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Pose, std::decay_t<U>>>>
    void SetTwist(U&& twist) noexcept
    {
        twist_ = std::forward<U>(twist);
    }

    /**
     * @brief Set acceleration.
     * @tparam U Type of acceleration, must be Pose.
     * @param accel New acceleration value.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Pose, std::decay_t<U>>>>
    void SetAccel(U&& accel) noexcept
    {
        accel_ = std::forward<U>(accel);
    }

    /**
     * @brief Set reference frame id.
     * @tparam U Type of frame id, must be std::string.
     * @param frame_id New frame id.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::string, std::decay_t<U>>>>
    void SetFrameId(U&& frame_id)
    {
        frame_id_ = std::forward<U>(frame_id);
    }

    /**
     * @brief Set child/body frame id.
     * @tparam U Type of frame id, must be std::string.
     * @param child_frame_id New child frame id.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::string, std::decay_t<U>>>>
    void SetChildFrameId(U&& child_frame_id)
    {
        child_frame_id_ = std::forward<U>(child_frame_id);
    }

public:
    /**
     * @brief Predict next odometry state by constant-acceleration model.
     * @tparam U Arithmetic type for dt input.
     * @param dt_seconds Delta time in seconds, must be finite and >= 0.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    void Predict(U dt_seconds)
    {
        const T dt = static_cast<T>(dt_seconds);
        if (!(dt >= static_cast<T>(0)) || !std::isfinite(static_cast<double>(dt)))
        {
            throw std::invalid_argument("dt_seconds must be finite and non-negative");
        }

        const T half_dt2 = static_cast<T>(0.5) * dt * dt;
        const Pose delta = twist_ * dt + accel_ * half_dt2;
        pose_.TranslatePose(delta.GetX(), delta.GetY(), delta.GetZ(),
                            delta.GetRoll(), delta.GetPitch(), delta.GetYaw());

        const Pose twist_delta = accel_ * dt;
        twist_.TranslatePose(twist_delta.GetX(), twist_delta.GetY(), twist_delta.GetZ(),
                             twist_delta.GetRoll(), twist_delta.GetPitch(), twist_delta.GetYaw());

        const auto dt_ns = static_cast<std::int64_t>(static_cast<double>(dt) * 1e9);
        timestamp_ = timestamp_.PlusNanoseconds(dt_ns);
    }

    /**
     * @brief Convert odometry state to a readable string.
     * @return String containing timestamp, frame ids and state vectors.
     */
    std::string ToString() const
    {
        return "Odometry{t=" + timestamp_.ToString() +
               ", frame_id=" + frame_id_ +
               ", child_frame_id=" + child_frame_id_ +
               ", pose=" + pose_.ToString() +
               ", twist=" + twist_.ToString() +
               ", accel=" + accel_.ToString() + "}";
    }
};

} // namespace structure
