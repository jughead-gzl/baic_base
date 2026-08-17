#pragma once

#include <type_traits>
#include <cmath>
#include <string>
#include <iostream>

namespace structure
{

template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
class Point2D
{
public:
    using value_type = T;
private:
    T x_{static_cast<T>(0)};
    T y_{static_cast<T>(0)};
public:
    Point2D(T x = static_cast<T>(0), T y = static_cast<T>(0)) : x_(x), y_(y)
    {

    }
    Point2D(const Point2D&) = default;
    Point2D& operator=(const Point2D&) = default;
    Point2D(Point2D&&) = default;
    Point2D& operator=(Point2D&&) = default;
public:
    const T& GetX() const noexcept
    {
        return x_;
    }
    const T& GetY() const noexcept
    {
        return y_;
    }
    T& GetX() noexcept
    {
        return x_;
    }
    T& GetY() noexcept
    {
        return y_;
    }
    T&operator[] (std::size_t index) noexcept
    {
        switch (index)
        {
        case 0: return x_;
        case 1: return y_;
        default: throw std::out_of_range("Index out of range for Point2D");
        }
    }

    const T&operator[] (std::size_t index) const noexcept
    {
        return const_cast<Point2D*>(this)->operator[](index);
    }

    void SetX(T x = static_cast<T>(0)) noexcept
    {
        x_ = x;
    }
    void SetY(T y = static_cast<T>(0)) noexcept
    {
        y_ = y;
    }
public:
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T DistOfChebyshev(U && point) const noexcept
    {
        return std::max(std::abs(x_ - std::forward<U>(point).GetX()), std::abs(y_ - std::forward<U>(point).GetY()));
    }
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T DistOfManhattan(U && point) const noexcept
    {
        return std::abs(x_ - std::forward<U>(point).GetX()) + std::abs(y_ - std::forward<U>(point).GetY());
    }
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T DistOfEuclidean(U && point) const noexcept
    {
        return std::sqrt(std::pow((x_ - std::forward<U>(point).GetX()), 2) + std::pow(y_ - std::forward<U>(point).GetY(), 2));
    }

    /**
     * @brief Translate the point by the given offsets in x and y directions.
     * @tparam U Type of the offsets, must be an arithmetic type
     * @param dx Offset in the x direction (default is 0).
     * @param dy Offset in the y direction (default is 0).
     * @return A reference to the current Point2D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& Translate(U dx = static_cast<U>(0), U dy = static_cast<U>(0)) noexcept
    {
        x_ += dx;
        y_ += dy;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset in the x direction.
     * @tparam U Type of the offset, must be an arithmetic type
     * @param dx Offset in the x direction (default is 0).
     * @return A reference to the current Point2D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& TranslateX(U dx = static_cast<U>(0)) noexcept
    {
        x_ += dx;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset in the y direction.
     * @tparam U Type of the offset, must be an arithmetic type
     * @param dy Offset in the y direction (default is 0).
     * @return A reference to the current Point2D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& TranslateY(U dy = static_cast<U>(0)) noexcept
    {
        y_ += dy;
        return *this;
    }

    /**
     * @brief Addition operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return A new Point2D that is the sum of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    Point2D operator+(U && other) const noexcept
    {
        return Point2D(x_ + std::forward<U>(other).GetX(), y_ + std::forward<U>(other).GetY());
    }

    /**
     * @brief Subtraction operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return A new Point2D that is the difference between this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    Point2D operator-(U && other) const noexcept
    {
        return Point2D(x_ - std::forward<U>(other).GetX(), y_ - std::forward<U>(other).GetY());
    }

    /**
     * @brief Scale the point by the given factors in x and y directions.
     * @tparam U Type of the scaling factors, must be an arithmetic type
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sy Scaling factor in the y direction (default is 1).
     * @return A reference to the current Point2D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& Scale(U sx = static_cast<U>(1), U sy = static_cast<U>(1)) noexcept
    {
        x_ *= sx;
        y_ *= sy;
        return *this;
    }

    /**
     * @brief Scale the point by the given factor in the x direction.
     * @tparam U Type of the scaling factor, must be an arithmetic type
     * @param sx Scaling factor in the x direction (default is 1).
     * @return A reference to the current Point2D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& ScaleX(U sx = static_cast<U>(1)) noexcept
    {
        x_ *= sx;
        return *this;
    }

    /**
     * @brief Scale the point by the given factor in the y direction.
     * @tparam U Type of the scaling factor, must be an arithmetic type
     * @param sy Scaling factor in the y direction (default is 1).
     * @return A reference to the current Point2D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& ScaleY(U sy = static_cast<U>(1)) noexcept
    {
        y_ *= sy;
        return *this;
    }

    /**
     * @brief Multiplication operator for Point2D.
     * @param scalar The scalar value to multiply with the point.
     * @return A new Point2D that is the result of the multiplication.
     */

    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    Point2D operator*(U scalar) const noexcept
    {
        return Point2D(x_ * scalar, y_ * scalar);
    }

    /**
     * @brief Rotate the point around the origin by the given angle in degrees.
     * @tparam U Type of the angle, must be an arithmetic type
     * @param angle Angle in degrees (default is 0).
     * @return A reference to the current Point2D object after rotation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point2D& Rotate(U angle = static_cast<U>(0)) noexcept
    {
        T radian = angle * static_cast<T>(M_PI) / static_cast<T>(180);
        T new_x = x_ * std::cos(radian) - y_ * std::sin(radian);
        T new_y = x_ * std::sin(radian) + y_ * std::cos(radian);
        x_ = new_x;
        y_ = new_y;
        return *this;
    }

    /**
     * @brief Rotate the point around the origin by the given angle in degrees in the clockwise direction.
     * @tparam U Type of the angle, must be an arithmetic type
     * @param angle Angle in degrees (default is 0).
     * @return A reference to the current Point2D object after rotation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    Point2D operator>(U angle) const noexcept
    {
        T radian = angle * static_cast<T>(M_PI) / static_cast<T>(180);
        T new_x = x_ * std::cos(radian) - y_ * std::sin(radian);
        T new_y = x_ * std::sin(radian) + y_ * std::cos(radian);
        return Point2D(new_x, new_y);
    }

    /**
     * @brief Rotate the point around the origin by the given angle in degrees in the counterclockwise direction.
     * @tparam U Type of the angle, must be an arithmetic type
     * @param angle Angle in degrees (default is 0).
     * @return A reference to the current Point2D object after rotation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    Point2D operator<(U angle) const noexcept
    {
        T radian = angle * static_cast<T>(M_PI) / static_cast<T>(180);
        T new_x = x_ * std::cos(radian) + y_ * std::sin(radian);
        T new_y = -x_ * std::sin(radian) + y_ * std::cos(radian);
        return Point2D(new_x, new_y);
    }

    /**
     * @brief Equality operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return true if the points are equal, false otherwise
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    bool operator==(U && other) const noexcept
    {
        if constexpr (std::is_integral_v<std::decay_t<T>> && std::is_integral_v<typename std::decay_t<U>::value_type>)
        {
            return x_ == std::forward<U>(other).GetX() && y_ == std::forward<U>(other).GetY();
        }
        else
        {
             return std::abs(x_ - std::forward<U>(other).GetX()) < std::numeric_limits<T>::epsilon() &&
                   std::abs(y_ - std::forward<U>(other).GetY()) < std::numeric_limits<T>::epsilon();
        }
    }

    /**
     * @brief Inequality operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return true if the points are not equal, false otherwise
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    bool operator!=(U && other) const noexcept
    {
        return !(*this == std::forward<U>(other));
    }


    /**
     * @brief Convert the Point2D to a string representation.
     * @return A string in the format "(x, y)"
     */
    const std::string ToString() const noexcept
    {
        return std::string("(") + std::to_string(x_) + ", " + std::to_string(y_) + ")";
    }

    /**
     * @brief Dot product operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return The dot product of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T Dot(U && other) const noexcept
    {
        return x_ * std::forward<U>(other).GetX() + y_ * std::forward<U>(other).GetY();
    }

    /**
     * @brief Cross product operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return The cross product of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    Point2D Cross(U && other) const noexcept
    {
        return Point2D(x_ * std::forward<U>(other).GetY() - y_ * std::forward<U>(other).GetX(), 0);
    }

    /**
     * @brief Get the length (magnitude) of the point vector.
     * @return The length of the vector from the origin to this point
     */
    T Length() const noexcept
    {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    /**
     * @brief Normalize the point to have a length of 1, if it is not already at the origin.
     * This modifies the point in place.
     */
    void Normalize() noexcept
    {
        const T length = Length();
        if (length > std::numeric_limits<T>::epsilon())
        {
            
            Scale(static_cast<T>(1) / length, static_cast<T>(1) / length);
        }
    }
    
};
};

