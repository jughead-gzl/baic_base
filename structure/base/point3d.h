#pragma once

#include "point2d.h"

namespace structure
{
template <typename T, typename = typename std::enable_if_t<std::is_arithmetic_v<T>>>
class Point3D : public Point2D<T>
{
public:
    using value_type = T;
private:
    T z_{static_cast<T>(0)};
public:
    /**
     * @brief Constructs a Point3D object with the given x, y, and z coordinates.
     * @param x The x-coordinate (default is 0).
     * @param y The y-coordinate (default is 0).
     * @param z The z-coordinate (default is 0).
     * @return A new Point3D object with the specified coordinates.
     */
    Point3D(T x = static_cast<T>(0), T y = static_cast<T>(0), T z = static_cast<T>(0)) : Point2D<T>(x, y), z_(z) {}
    /**
     * @brief Constructs a Point3D object from a Point2D object and an optional z-coordinate.
     * @param point The Point2D object to copy the x and y coordinates from.
     * @param z The z-coordinate (default is 0).
     * @return A new Point3D object with the same x and y coordinates as the Point2D object and the specified z-coordinate.
     */
    Point3D(const Point2D<T>& point, T z = static_cast<T>(0)) : Point3D(point.GetX(), point.GetY(), z) {}
    Point3D(const Point3D&) = default;
    Point3D& operator=(const Point3D&) = default;
    Point3D(Point3D&&) = default;
    Point3D& operator=(Point3D&&) = default;
public:
    /**
     * @brief Get the z-coordinate of the Point3D object.
     * @return The z-coordinate of the Point3D object.
     */
    const T& GetZ() const noexcept
    {
        return z_;
    }
    /**
     * @brief Get the z-coordinate of the Point3D object.
     * @return The z-coordinate reference of the Point3D object.
     */
    T& GetZ() noexcept
    {
        return z_;
    }
    /**
     * @brief Set the z-coordinate of the Point3D object.
     * @param z The new z-coordinate (default is 0).
     */
    void SetZ(T z = static_cast<T>(0))
    {
        z_ = z;
    }

    /**
     * @brief Access the coordinates of the Point3D object by index.
     * @param index The index of the coordinate (0 for x, 1 for y
     * and 2 for z).
     * @return A reference to the coordinate at the specified index.
     */
    T&operator[] (std::size_t index) noexcept
    {
        switch (index)
        {
        case 0: return Point2D<T>::GetX();
        case 1: return Point2D<T>::GetY();
        case 2: return z_;
        default: throw std::out_of_range("Index out of range for Point3D");
        }
    }
    /**
     * @brief Access the coordinates of the Point3D object by index (const version).
     * @param index The index of the coordinate (0 for x, 1 for y and 2 for z).
     * @return A const reference to the coordinate at the specified index.
     */
    const T&operator[] (std::size_t index) const noexcept
    {
        return const_cast<Point3D*>(this)->operator[](index);
    }
public:
    /**
     * @brief Calculate the Chebyshev distance between this Point3D and another point.
     * @tparam U Type of the other point, must be derived from Point3D
     * @param point The other Point3D object to calculate the distance to.
     * @return The Chebyshev distance between this point and the other point.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T DistOfChebyshev(U && point) const noexcept
    {
        return std::max(Point2D<T>::DistOfChebyshev(point), std::abs(z_ - point.GetZ()));
    }

    /**
     * @brief Calculate the Manhattan distance between this Point3D and another point.
     * @tparam U Type of the other point, must be derived from Point3D
     * @param point The other Point3D object to calculate the distance to.
     * @return The Manhattan distance between this point and the other point.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T DistOfManhattan(U && point) const noexcept
    {
        return Point2D<T>::DistOfManhattan(point) + std::abs(z_ - point.GetZ());
    }
    /**
     * @brief Calculate the Euclidean distance between this Point3D and another point.
     * @tparam U Type of the other point, must be derived from Point3D
     * @param point The other Point3D object to calculate the distance to.
     * @return The Euclidean distance between this point and the other point.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point3D, std::decay_t<U>>>>
    T DistOfEuclidean(U && point) const noexcept
    {
        return std::sqrt(std::pow((Point2D<T>::GetX() - point.GetX()), 2) + std::pow(Point2D<T>::GetY() - point.GetY(), 2) + std::pow(z_ - point.GetZ(), 2));
    }

    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D<T>, std::decay_t<U>>>>
    T Dist2DOfChebyshev(U && point) const noexcept
    {
        return Point2D<T>::DistOfChebyshev(point);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D<T>, std::decay_t<U>>>>
    T Dist2DOfManhattan(U && point) const noexcept
    {
        return Point2D<T>::DistOfManhattan(point);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D<T>, std::decay_t<U>>>>
    T Dist2DOfEuclidean(U && point) const noexcept
    {
        return Point2D<T>::DistOfEuclidean(point);
    }

    /**
     * @brief Translate the point by the given offsets in in x, y, and z directions.
     * @tparam U Type of the offsets, must be an arithmetic type
     * @param dx Offset in the x direction (default is 0).
     * @param dy Offset in the y direction (default is 0).
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& Translate(U dx = static_cast<U>(0), U dy = static_cast<U>(0), U dz = static_cast<U>(0)) noexcept
    {
        Point2D<T>::Translate(dx, dy);
        z_ += dz;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset in the z direction.
     * @tparam U Type of the offset, must be an arithmetic type
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& TranslateZ(U dz = static_cast<U>(0)) noexcept
    {
        z_ += dz;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset x and y directions.
     * @tparam U Type of the offsets, must be an arithmetic type
     * @param dx Offset in the x direction (default is 0).
     * @param dy Offset in the y direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& TranslateXY(U dx = static_cast<U>(0), U dy = static_cast<U>(0)) noexcept
    {
        Point2D<T>::Translate(dx, dy);
        return *this;
    }


    /**
     * @brief Translate the point by the given offset x and z directions.
     * @tparam U Type of the offsets, must be an arithmetic type
     * @param dx Offset in the x direction (default is 0).
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& TranslateXZ(U dx = static_cast<U>(0), U dz = static_cast<U>(0)) noexcept
    {
        Point2D<T>::TranslateX(dx);
        z_ += dz;
        return *this;
    }

    /**
     * @brief Translate the point by the given offset y and z directions.
     * @tparam U Type of the offsets, must be an arithmetic type
     * @param dy Offset in the y direction (default is 0).
     * @param dz Offset in the z direction (default is 0).
     * @return A reference to the current Point3D object after translation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& TranslateYZ(U dy = static_cast<U>(0), U dz = static_cast<U>(0)) noexcept
    {
        Point2D<T>::TranslateY(dy);
        z_ += dz;
        return *this;
    }

    /**
    /**
     * @brief Rotate the point around the origin by the given angle in degrees.
     * @tparam U Type of the angle, must be an arithmetic type
     * @param angle Angle in degrees (default is 0).
     * @return A reference to the current Point3D object after rotation.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& Rotate(U angle = static_cast<U>(0)) noexcept
    {
        Point2D<T>::Rotate(angle);
        return *this;
    }
    /**
     * @brief Scale the point by the given factors in x, y, and z directions.
     * @tparam U Type of the scaling factors, must be an arithmetic type
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sy Scaling factor in the y direction (default is 1).
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& Scale(U sx = static_cast<U>(1), U sy = static_cast<U>(1), U sz = static_cast<U>(1)) noexcept
    {
        Point2D<T>::Scale(sx, sy);
        z_ *= sz;
        return *this;
    }


    /**
     * @brief Scale the point by the given factor in the z direction.
     * @tparam U Type of the scaling factor, must be an arithmetic type
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& ScaleZ(U sz = static_cast<U>(1)) noexcept
    {
        z_ *= sz;
        return *this;
    }

    /**
     * @brief Scale the point by the given factors in x and y directions.
     * @tparam U Type of the scaling factors, must be an arithmetic type
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sy Scaling factor in the y direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& ScaleXY(U sx = static_cast<U>(1), U sy = static_cast<U>(1)) noexcept
    {
        Point2D<T>::Scale(sx, sy);
        return *this;
    }

    /**
     * @brief Scale the point by the given factors in x and z directions.
     * @tparam U Type of the scaling factors, must be an arithmetic type
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& ScaleXZ(U sx = static_cast<U>(1), U sz = static_cast<U>(1)) noexcept
    {
        Point2D<T>::ScaleX(sx);
        ScaleZ(sz);
        return *this;
    }

    /**
     * @brief Scale the point by the given factors in y and z directions.
     * @tparam U Type of the scaling factors, must be an arithmetic type
     * @param sy Scaling factor in the y direction (default is 1).
     * @param sz Scaling factor in the z direction (default is 1).
     * @return A reference to the current Point3D object after scaling.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    const Point3D& ScaleYZ(U sy = static_cast<U>(1), U sz = static_cast<U>(1)) noexcept
    {
        Point2D<T>::ScaleY(sy);
        ScaleZ(sz);
        return *this;
    }

    /**
     * @brief Multiplication operator for Point3D.
     * @tparam U Type of the scalar, must be an arithmetic type
     * @param scalar The scalar value to multiply with the point.
     * @return A new Point3D that is the result of the multiplication.
     */
    template <typename U, typename = typename std::enable_if_t<std::is_arithmetic_v<std::decay_t<U>>>>
    Point3D operator*(U scalar) const noexcept
    {
        return Point3D(Point2D<T>::operator*(scalar), z_ * scalar);
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
