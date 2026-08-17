#pragma once

#include "point3d.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace structure
{

/**
 * @brief Six degree-of-freedom point representation.
 *
 * This type extends Point3D by adding orientation components:
 * - roll  : rotation around x-axis
 * - pitch : rotation around y-axis
 * - yaw   : rotation around z-axis
 *
 * Coordinate layout:
 * - Position: (x, y, z) inherited from Point3D
 * - Orientation: (roll, pitch, yaw)
 */
template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
class Point6D : public Point3D<T>
{
public:
    /** @brief Scalar value type. */
    using value_type = T;
    /** @brief Base class alias for position-related operations. */
    using Base = Point3D<T>;

private:
    T roll_{static_cast<T>(0)};
    T pitch_{static_cast<T>(0)};
    T yaw_{static_cast<T>(0)};

public:
    /**
     * @brief Construct a 6-DOF point.
     * @param x Position x.
     * @param y Position y.
     * @param z Position z.
     * @param roll Orientation roll.
     * @param pitch Orientation pitch.
     * @param yaw Orientation yaw.
     */
    Point6D(T x = static_cast<T>(0), T y = static_cast<T>(0), T z = static_cast<T>(0), T roll = static_cast<T>(0), T pitch = static_cast<T>(0), T yaw = static_cast<T>(0)) noexcept
        : Base(x, y, z), roll_(roll), pitch_(pitch), yaw_(yaw)
    {
    }

    /**
     * @brief Construct from a Point3D plus orientation.
     * @param point Position source.
     * @param roll Orientation roll.
     * @param pitch Orientation pitch.
     * @param yaw Orientation yaw.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Point3D<T>, std::decay_t<U>>>>
    Point6D(U && point, T roll = static_cast<T>(0), T pitch = static_cast<T>(0), T yaw = static_cast<T>(0)) noexcept
        : Point6D(std::forward<U>(point).GetX(), std::forward<U>(point).GetY(), std::forward<U>(point).GetZ(), roll, pitch, yaw)
    {
    }

    Point6D(const Point6D&) = default;
    Point6D& operator=(const Point6D&) = default;
    Point6D(Point6D&&) = default;
    Point6D& operator=(Point6D&&) = default;

public:
    /** @brief Get roll component (const). */
    const T& GetRoll() const noexcept
    {
        return roll_;
    }

    /** @brief Get roll component (mutable). */
    T& GetRoll() noexcept
    {
        return roll_;
    }
    
    /** @brief Get pitch component (const). */
    const T& GetPitch() const noexcept
    {
        return pitch_;
    }

    /** @brief Get pitch component (mutable). */
    T& GetPitch() noexcept
    {
        return pitch_;
    }
    
    /** @brief Get yaw component (const). */
    const T& GetYaw() const noexcept
    {
        return yaw_;
    }

    /** @brief Get yaw component (mutable). */
    T& GetYaw() noexcept
    {
        return yaw_;
    }

public:
    /**
     * @brief Set pitch component.
     * @param pitch New pitch value.
     */
    void SetPitch(T pitch = static_cast<T>(0)) noexcept
    {
        pitch_ = pitch;
    }

    /**
     * @brief Set roll component.
     * @param roll New roll value.
     */
    void SetRoll(T roll = static_cast<T>(0)) noexcept
    {
        roll_ = roll;
    }

    /**
     * @brief Set yaw component.
     * @param yaw New yaw value.
     */
    void SetYaw(T yaw = static_cast<T>(0)) noexcept
    {
        yaw_ = yaw;
    }

    /**
     * @brief Set orientation tuple (roll, pitch, yaw).
     * @param roll New roll value.
     * @param pitch New pitch value.
     * @param yaw New yaw value.
     */
    void SetRPY(T roll = static_cast<T>(0), T pitch = static_cast<T>(0), T yaw = static_cast<T>(0)) noexcept
    {
        roll_ = roll;
        pitch_ = pitch;
        yaw_ = yaw;
    }

public:
    /**
     * @brief Access one of the 6 components by index.
     * @param index Component index:
     * 0->x, 1->y, 2->z, 3->roll, 4->pitch, 5->yaw.
     * @return Mutable reference to selected component.
     * @throws std::out_of_range if index is not in [0, 5].
     */
    T& operator[](std::size_t index)
    {
        switch (index)
        {
        case 0: return Base::operator[](0);
        case 1: return Base::operator[](1);
        case 2: return Base::operator[](2);
        case 3: return roll_;
        case 4: return pitch_;
        case 5: return yaw_;
        default: throw std::out_of_range("Index out of range for Point6D");
        }
    }

    /**
     * @brief Const access to one of the 6 components by index.
     * @param index Component index:
     * 0->x, 1->y, 2->z, 3->roll, 4->pitch, 5->yaw.
     * @return Const reference to selected component.
     * @throws std::out_of_range if index is not in [0, 5].
     */
    const T& operator[](std::size_t index) const
    {
        return const_cast<Point6D*>(this)->operator[](index);
    }

public:
    /**
     * @brief Translate position part (x,y,z).
     * @tparam U Arithmetic delta type.
     * @param dx Delta x.
     * @param dy Delta y.
     * @param dz Delta z.
     * @return Reference to current object.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point6D& Translate(U dx = static_cast<U>(0), U dy = static_cast<U>(0), U dz = static_cast<U>(0)) noexcept
    {
        Base::Translate(dx, dy, dz);
        return *this;
    }

    /**
     * @brief Translate orientation part (roll,pitch,yaw).
     * @tparam U Arithmetic delta type.
     * @param droll Delta roll.
     * @param dpitch Delta pitch.
     * @param dyaw Delta yaw.
     * @return Reference to current object.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point6D& TranslateRPY(U droll = static_cast<U>(0), U dpitch = static_cast<U>(0), U dyaw = static_cast<U>(0)) noexcept
    {
        roll_ += droll;
        pitch_ += dpitch;
        yaw_ += dyaw;
        return *this;
    }

    /**
     * @brief Translate both position and orientation in one call.
     * @tparam U Arithmetic delta type.
     * @param dx Delta x.
     * @param dy Delta y.
     * @param dz Delta z.
     * @param droll Delta roll.
     * @param dpitch Delta pitch.
     * @param dyaw Delta yaw.
     * @return Reference to current object.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point6D& TranslatePose(U dx = static_cast<U>(0), U dy = static_cast<U>(0), U dz = static_cast<U>(0),
                                 U droll = static_cast<U>(0), U dpitch = static_cast<U>(0), U dyaw = static_cast<U>(0)) noexcept
    {
        Base::Translate(dx, dy, dz);
        roll_ += droll;
        pitch_ += dpitch;
        yaw_ += dyaw;
        return *this;
    }

public:
    /**
     * @brief Component-wise addition in 6D.
     * @tparam U Type derived from Point6D.
     * @param other Right-hand operand.
     * @return New 6D point equal to this + other.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point6D, std::decay_t<U>>>>
    Point6D operator+(U&& other) const noexcept
    {
        return Point6D(Base::operator+(std::forward<U>(other)),
            roll_ + std::forward<U>(other).GetRoll(),
            pitch_ + std::forward<U>(other).GetPitch(),
            yaw_ + std::forward<U>(other).GetYaw());
    }

    /**
     * @brief Component-wise subtraction in 6D.
     * @tparam U Type derived from Point6D.
     * @param other Right-hand operand.
     * @return New 6D point equal to this - other.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point6D, std::decay_t<U>>>>
    Point6D operator-(U&& other) const noexcept
    {
        return Point6D(Base::operator-(std::forward<U>(other)),
            roll_ - std::forward<U>(other).GetRoll(),
            pitch_ - std::forward<U>(other).GetPitch(),
            yaw_ - std::forward<U>(other).GetYaw());
    }

public:
    /**
     * @brief Equality comparison for all 6 components.
     * @tparam U Type derived from Point6D.
     * @param other Right-hand operand.
     * @return true if all components are equal.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point6D, std::decay_t<U>>>>
    bool operator==(U&& other) const noexcept
    {
        return Base::operator==(std::forward<U>(other)) &&
               roll_ == std::forward<U>(other).GetRoll() &&
               pitch_ == std::forward<U>(other).GetPitch() &&
               yaw_ == std::forward<U>(other).GetYaw();
    }

    /**
     * @brief Inequality comparison for 6D point.
     * @tparam U Type derived from Point6D.
     * @param other Right-hand operand.
     * @return true if any component differs.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point6D, std::decay_t<U>>>>
    bool operator!=(U&& other) const noexcept
    {
        return !(*this == std::forward<U>(other));
    }

public:
    /**
     * @brief Convert 6D point to string.
     * @return String in format "(x, y, z, roll, pitch, yaw)".
     */
    std::string ToString() const noexcept
    {
        return "(" + std::to_string(Base::GetX()) + ", " + std::to_string(Base::GetY()) + ", " + std::to_string(Base::GetZ()) +
               ", " + std::to_string(roll_) + ", " + std::to_string(pitch_) + ", " + std::to_string(yaw_) + ")";
    }
};

} // namespace structure
