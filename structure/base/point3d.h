#pragma once

#include "point2d.h"

namespace structure
{
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
class Point3D : public Point2D<T>
{
public:
    using value_type = T;
private:
    T z_{static_cast<T>(0)};
public:
    Point3D(T x = static_cast<T>(0), T y = static_cast<T>(0), T z = static_cast<T>(0)) : Point2D<T>(x, y), z_(z)
    {

    }
    Point3D(const Point2D<T>& point, T z = static_cast<T>(0)) : Point3D(point.GetX(), point.GetY(), z)
    {

    }
    Point3D(const Point3D&) = default;
    Point3D& operator=(const Point3D&) = default;
    Point3D(Point3D&&) = default;
    Point3D& operator=(Point3D&&) = default;
public:
    const T& GetZ() const noexcept
    {
        return z_;
    }
    T& GetZ() noexcept
    {
        return z_;
    }
    void SetZ(T z = static_cast<T>(0))
    {
        z_ = z;
    }
public:
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T DistOfChebyshev(U && point) const noexcept
    {
        return std::max(Point2D<T>::DistOfChebyshev(point), std::abs(z_ - point.GetZ()));
    }
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T DistOfManhattan(U && point) const noexcept
    {
        return Point2D<T>::DistOfManhattan(point) + std::abs(z_ - point.GetZ());
    }
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T DistOfEuclidean(U && point) const noexcept
    {
        return std::sqrt(std::pow((Point2D<T>::GetX() - point.GetX()), 2) + std::pow(Point2D<T>::GetY() - point.GetY(), 2) + std::pow(z_ - point.GetZ(), 2));
    }

    /**
     * @brief Translate the point by the given offsets in in x, y, and z directions.
     * @param dx Offset in the x direction (default is 0).
     * @param dy Offset in the y direction (default is 0).
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    const Point3D& Translate(T dx = static_cast<T>(0), T dy = static_cast<T>(0), T dz = static_cast<T>(0)) noexcept
    {
        Point2D<T>::Translate(dx, dy);
        z_ += dz;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset in the z direction.
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    const Point3D& TranslateZ(T dz = static_cast<T>(0)) noexcept
    {
        z_ += dz;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset x and y directions.
     * @param dx Offset in the x direction (default is 0).
     * @param dy Offset in the y direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    const Point3D& TranslateXY(T dx = static_cast<T>(0), T dy = static_cast<T>(0)) noexcept
    {
        Point2D<T>::Translate(dx, dy);
        return *this;
    }


    /**
     * @brief Translate the point by the given offset x and z directions.
     * @param dx Offset in the x direction (default is 0).
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    const Point3D& TranslateXZ(T dx = static_cast<T>(0), T dz = static_cast<T>(0)) noexcept
    {
        Point2D<T>::TranslateX(dx);
        z_ += dz;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset y and z directions.
     * @param dy Offset in the y direction (default is 0).
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    const Point3D& TranslateYZ(T dy = static_cast<T>(0), T dz = static_cast<T>(0)) noexcept
    {
        Point2D<T>::TranslateY(dy);
        z_ += dz;
        return *this;
    }

    /**
     * @brief Rotate the point around the origin by the given angle in degrees.
     * @param angle Angle in degrees (default is 0).
     * @return A reference to the current Point3D object after rotation.
     */
    const Point3D& Rotate(T angle = static_cast<T>(0)) noexcept
    {
        Point2D<T>::Rotate(angle);
        return *this;
    }
    /**
     * @brief Scale the point by the given factors in x, y, and z directions.
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sy Scaling factor in the y direction (default is 1).
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    const Point3D& Scale(T sx = static_cast<T>(1), T sy = static_cast<T>(1), T sz = static_cast<T>(1)) noexcept
    {
        Point2D<T>::Scale(sx, sy);
        z_ *= sz;
        return *this;
    }


    /**
     * @brief Scale the point by the given factor in the z direction.
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    const Point3D& ScaleZ(T sz = static_cast<T>(1)) noexcept
    {
        z_ *= sz;
        return *this;
    }

    /**
     * @brief Scale the point by the given factors in x and y directions.
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sy Scaling factor in the y direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    const Point3D& ScaleXY(T sx = static_cast<T>(1), T sy = static_cast<T>(1)) noexcept
    {
        Point2D<T>::Scale(sx, sy);
        return *this;
    }

    /**
     * @brief Scale the point by the given factors in x and z directions.
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    const Point3D& ScaleXZ(T sx = static_cast<T>(1), T sz = static_cast<T>(1)) noexcept
    {
        Point2D<T>::ScaleX(sx);
        ScaleZ(sz);
        return *this;
    }

    /**
     * @brief Scale the point by the given factors in y and z directions.
     * @param sy Scaling factor in the y direction (default is 1).
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    const Point3D& ScaleYZ(T sy = static_cast<T>(1), T sz = static_cast<T>(1)) noexcept
    {
        Point2D<T>::ScaleY(sy);
        ScaleZ(sz);
        return *this;
    }

    /**
     * @brief Multiplication operator for Point3D.
     * @param scalar The scalar value to multiply with the point.
     * @return A new Point3D that is the result of the multiplication.
     */
    Point3D operator*(T scalar) const noexcept
    {
        return Point3D(Point2D<T>::operator*(scalar), z_ * scalar);
    }

    /**
     * @brief Three-way comparison operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
      decltype(auto) operator<=>(U && other) const noexcept
    {
        if (Point2D<T>::GetX() < std::forward<U>(other).GetX()) 
            return std::strong_ordering::less;
        if (Point2D<T>::GetX() > std::forward<U>(other).GetX()) 
            return std::strong_ordering::greater;
        if (Point2D<T>::GetY() < std::forward<U>(other).GetY()) 
            return std::strong_ordering::less;
        if (Point2D<T>::GetY() > std::forward<U>(other).GetY()) 
            return std::strong_ordering::greater;
        if (z_ < std::forward<U>(other).GetZ()) 
            return std::strong_ordering::less;
        if (z_ > std::forward<U>(other).GetZ()) 
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    /**
     * @brief Equality operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     * @return true if the points are equal, false otherwise
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    bool operator==(U && other) const noexcept
    {
        return Point2D<T>::operator==(std::forward<U>(other)) && z_ == std::forward<U>(other).GetZ();
    }

    /**
     * @brief Inequality operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     * @return true if the points are not equal, false otherwise
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    bool operator!=(U && other) const noexcept
    {
        return !(*this == std::forward<U>(other));
    }

    /**
     * @brief Addition operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    Point3D operator+(U && other) const noexcept
    {
        return Point3D(Point2D<T>::operator+(std::forward<U>(other)), z_ + std::forward<U>(other).GetZ());
    }

    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    Point3D& Add(U && other) noexcept
    {
        Point2D<T>::Add(std::forward<U>(other));
        z_ += std::forward<U>(other).GetZ();
        return *this;
    }

    /**
     * @brief Subtraction operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    Point3D operator-(U && other) const noexcept
    {
        return Point3D(Point2D<T>::operator-(std::forward<U>(other)), z_ - std::forward<U>(other).GetZ());
    }

    /**
     * @brief Subtract another Point3D from this point and return the result as a new Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     * @return A new Point3D that is the difference between this point and the other
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    Point3D& Sub(U && other) noexcept
    {
        Point2D<T>::Sub(std::forward<U>(other));
        z_ -= std::forward<U>(other).GetZ();
        return *this;
    }

    /**
     * @brief Convert the Point3D to a string representation.
     * @return A string in the format "(x, y, z)"
     */
    std::string ToString() const noexcept
    {
        return "(" + std::to_string(Point2D<T>::GetX()) + ", " + std::to_string(Point2D<T>::GetY()) + ", " + std::to_string(z_) + ")";
    }

    /**
     * @brief Dot product operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     * @return The dot product of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T Dot(U && other) const noexcept
    {
        return Point2D<T>::Dot(std::forward<U>(other)) + z_ * std::forward<U>(other).GetZ();
    }

    /**
     * @brief Cross product operator for Point3D.
     * @tparam U Type of the other point, must be derived from Point3D
     * @return The cross product of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    Point3D Cross(U && other) const noexcept
    {
        T cross_x = Point2D<T>::GetY() * std::forward<U>(other).GetZ() - z_ * std::forward<U>(other).GetY();
        T cross_y = z_ * std::forward<U>(other).GetX() - Point2D<T>::GetX() * std::forward<U>(other).GetZ();
        T cross_z = Point2D<T>::GetX() * std::forward<U>(other).GetY() - Point2D<T>::GetY() * std::forward<U>(other).GetX();
        return Point3D(cross_x, cross_y, cross_z);
    }

    /**
     * @brief Get the length (magnitude) of the point vector.
     * @return The length of the vector from the origin to this point
     */
    T Length() const noexcept
    {
        return std::sqrt(std::pow(Point2D<T>::Length(), 2) + z_ * z_);
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
            Scale(static_cast<T>(1) / length, static_cast<T>(1) / length, static_cast<T>(1) / length);
        }
    }

    
};
};
