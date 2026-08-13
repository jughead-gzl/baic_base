#pragma once

#include "..\..\structure\base\point6d.h"
#include "..\..\structure\base\line2d.h"
#include "..\..\structure\base\line3d.h"
#include <cmath>
#include <type_traits>
#include <iostream>

namespace algorithm 
{
/**
 * @brief Calculates the Chebyshev distance between two points.
 * 
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The Chebyshev distance between the two points.
 */
template <typename T>
decltype(auto) DistOfChebyshev(T && p1, T && p2) 
{
    // Check if the points are of the same type and are Point2D
    if constexpr (std::is_same_v<std::decay_t<T>, structure::Point2D<typename std::decay_t<T>::value_type>>)
    {
        return std::max(std::abs(std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX()), std::abs(std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY()));
    }
    // Check if the points are of the same type and are Point3D
    else if constexpr (std::is_same_v<std::decay_t<T>, structure::Point3D<typename std::decay_t<T>::value_type>>)
    {
        return std::max(std::max(std::abs(std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX()), std::abs(std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY())), std::abs(std::forward<T>(p1).GetZ() - std::forward<T>(p2).GetZ()));
    }
    // Check if the points are of the same type and are Point6D
    else if constexpr (std::is_same_v<std::decay_t<T>, structure::Point6D<typename std::decay_t<T>::value_type>>)
    {
        return std::max(std::max(std::abs(std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX()), std::abs(std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY())), std::abs(std::forward<T>(p1).GetZ() - std::forward<T>(p2).GetZ()));
    }
    else
    {
        // Static assert to ensure that the points are of a supported type (Point2D, Point3D, or Point6D)
        static_assert(
            std::is_same_v<std::decay_t<T>, 
            structure::Point2D<typename std::decay_t<T>::value_type>> || std::is_same_v<std::decay_t<T>, structure::Point3D<typename std::decay_t<T>::value_type>> || std::is_same_v<std::decay_t<T>, structure::Point6D<typename std::decay_t<T>::value_type>>, 
            "Unsupported point type");
    }
};

/**
 * @brief Calculates the Manhattan distance between two points.
 * 
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The Manhattan distance between the two points.
 */
template <typename T>
decltype(auto) DistOfManhattan(T && p1, T && p2) 
{
    // Check if the points are of the same type and are Point2D
    if constexpr (std::is_same_v<std::decay_t<T>, structure::Point2D<typename std::decay_t<T>::value_type>>)
    {
        return std::abs(std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX()) + std::abs(std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY());
    }
    // Check if the points are of the same type and are Point3D
    else if constexpr (std::is_same_v<std::decay_t<T>, structure::Point3D<typename std::decay_t<T>::value_type>>)
    {
        return std::abs(std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX()) + std::abs(std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY()) + std::abs(std::forward<T>(p1).GetZ() - std::forward<T>(p2).GetZ());
    }
    // Check if the points are of the same type and are Point6D
    else if constexpr (std::is_same_v<std::decay_t<T>, structure::Point6D<typename std::decay_t<T>::value_type>>)
    {
        return std::abs(std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX()) + std::abs(std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY()) + std::abs(std::forward<T>(p1).GetZ() - std::forward<T>(p2).GetZ());
    }
    else
    {
        // Static assert to ensure that the points are of a supported type (Point2D, Point3D, or Point6D)
        static_assert(
            std::is_same_v<std::decay_t<T>, 
            structure::Point2D<typename std::decay_t<T>::value_type>> || std::is_same_v<std::decay_t<T>, structure::Point3D<typename std::decay_t<T>::value_type>> || std::is_same_v<std::decay_t<T>, structure::Point6D<typename std::decay_t<T>::value_type>>, 
            "Unsupported point type");
    }
};

/**
 * @brief Calculates the Euclidean distance between two points.
 * 
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The Euclidean distance between the two points.
 */
template <typename T>
decltype(auto) DistOfEuclidean(T && p1, T && p2) 
{
    // Check if the points are of the same type and are Point2D
    if constexpr (std::is_same_v<std::decay_t<T>, structure::Point2D<typename std::decay_t<T>::value_type>>)
    {
        auto dx = std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX();
        auto dy = std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY();
        return std::sqrt(dx * dx + dy * dy);
    }
    // Check if the points are of the same type and are Point3D
    else if constexpr (std::is_same_v<std::decay_t<T>, structure::Point3D<typename std::decay_t<T>::value_type>>)
    {
        auto dx = std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX();
        auto dy = std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY();
        auto dz = std::forward<T>(p1).GetZ() - std::forward<T>(p2).GetZ();
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    // Check if the points are of the same type and are Point6D
    else if constexpr (std::is_same_v<std::decay_t<T>, structure::Point6D<typename std::decay_t<T>::value_type>>)
    {
        auto dx = std::forward<T>(p1).GetX() - std::forward<T>(p2).GetX();
        auto dy = std::forward<T>(p1).GetY() - std::forward<T>(p2).GetY();
        auto dz = std::forward<T>(p1).GetZ() - std::forward<T>(p2).GetZ();
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    else
    {
        // Static assert to ensure that the points are of a supported type (Point2D, Point3D, or Point6D)
        static_assert(
            std::is_same_v<std::decay_t<T>, 
            structure::Point2D<typename std::decay_t<T>::value_type>> || std::is_same_v<std::decay_t<T>, structure::Point3D<typename std::decay_t<T>::value_type>> || std::is_same_v<std::decay_t<T>, structure::Point6D<typename std::decay_t<T>::value_type>>, 
            "Unsupported point type");
    }
};

/**
 * @brief Calculates the distance from a point to a line.
 * 
 * @param point The point.
 * @param line The line.
 * @return The distance from the point to the line.
 */
template <typename T1, typename T2>
decltype(auto) DistFromPointToLine(T1 && point, T2 && line) 
{
    // Check if the point and line are of compatible types (Point2D with Line2D or Point3D with Line3D)
    if constexpr (std::is_same_v<std::decay_t<T1>, structure::Point2D<typename std::decay_t<T1>::value_type>> && std::is_same_v<std::decay_t<T2>, structure::Line2D<typename std::decay_t<T2>::value_type>> ||
                std::is_same_v<std::decay_t<T1>, structure::Point3D<typename std::decay_t<T1>::value_type>> && std::is_same_v<std::decay_t<T2>, structure::Line3D<typename std::decay_t<T2>::value_type>>)
    {
        return std::forward<T2>(line).DistFromPoint(std::forward<T1>(point));
    } 
    else 
    {
        // Static assert to ensure that the point and line are of compatible types
        static_assert(
            (std::is_same_v<std::decay_t<T1>, structure::Point2D<typename std::decay_t<T1>::value_type>> && std::is_same_v<std::decay_t<T2>, structure::Line2D<typename std::decay_t<T2>::value_type>>) ||
            (std::is_same_v<std::decay_t<T1>, structure::Point3D<typename std::decay_t<T1>::value_type>> && std::is_same_v<std::decay_t<T2>, structure::Line3D<typename std::decay_t<T2>::value_type>>),
            "Unsupported point or line type"
        );
    }
};


} // namespace algorithm


template <typename T, typename = typename std::enable_if_t<std::is_base_of_v<structure::Point2D<typename std::decay_t<T>::value_type>, std::decay_t<T>>>>
std::ostream& operator<<(std::ostream& os, T && point)
{
    os << std::forward<T>(point).ToString();
    return os;
}
