#pragma once

#include "..\algorithm\alam\odometry.h"
#include "timestamp.h"

#include <cmath>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace structure
{

/**
 * @brief Planning path represented by an ordered list of odometry states.
 *
 * Each element is an Odometry state, and path length is computed from pose positions.
 */
template <typename T, typename ContainerT = std::vector<Odometry<T>>,
          typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
class PlanningPath
{
public:
    using value_type = T;
    using PathPoint = Odometry<T>;
    using PathPoints = ContainerT;

private:
    PathPoints path_points{};
    Timestamp timestamp_{Timestamp::NowSteady()};
    std::string frame_id_{"map"};

public:
    PlanningPath() = default;

    template <typename U1, typename U2, typename U3,
              typename = typename std::enable_if_t<std::is_same_v<Timestamp, std::decay_t<U1>> &&
                                                   std::is_same_v<PathPoints, std::decay_t<U2>> &&
                                                   std::is_same_v<std::string, std::decay_t<U3>>>>
    PlanningPath(U1&& timestamp, U2&& waypoints, U3&& frame_id = "map")
        : path_points(std::forward<U2>(waypoints)), timestamp_(std::forward<U1>(timestamp)), frame_id_(std::forward<U3>(frame_id))
    {
    }

    template <typename U1, typename U2,
              typename = typename std::enable_if_t<std::is_same_v<PathPoints, std::decay_t<U1>> &&
                                                   std::is_same_v<std::string, std::decay_t<U2>>>>
    PlanningPath(U1&& waypoints, U2&& frame_id = "map")
        : path_points(std::forward<U1>(waypoints)), frame_id_(std::forward<U2>(frame_id))
    {
    }

    PlanningPath(const PlanningPath&) = default;
    PlanningPath& operator=(const PlanningPath&) = default;
    PlanningPath(PlanningPath&&) = default;
    PlanningPath& operator=(PlanningPath&&) = default;
    ~PlanningPath() = default;

public:
    const PathPoints& GetPathPoints() const noexcept
    {
        return path_points;
    }

    PathPoints& GetPathPoints() noexcept
    {
        return path_points;
    }

    const std::string& GetFrameId() const noexcept
    {
        return frame_id_;
    }

    const Timestamp& GetTimestamp() const noexcept
    {
        return timestamp_;
    }

    Timestamp& GetTimestamp() noexcept
    {
        return timestamp_;
    }

public:
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<PathPoints, std::decay_t<U>>>>
    void SetPathPoints(U&& waypoints) noexcept
    {
        path_points = std::forward<U>(waypoints);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Timestamp, std::decay_t<U>>>>
    void SetTimestamp(U&& timestamp) noexcept
    {
        timestamp_ = std::forward<U>(timestamp);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::string, std::decay_t<U>>>>
    void SetFrameId(U&& frame_id)
    {
        frame_id_ = std::forward<U>(frame_id);
    }

public:
    bool Empty() const noexcept
    {
        return path_points.empty();
    }

    std::size_t Size() const noexcept
    {
        return path_points.size();
    }

    void Clear() noexcept
    {
        path_points.clear();
    }

    void Reserve(std::size_t count)
    {
        path_points.reserve(count);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<PathPoint, std::decay_t<U>>>>
    void PushBack(U&& waypoint)
    {
        path_points.push_back(std::forward<U>(waypoint));
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<PathPoint, std::decay_t<U>>>>
    void PushFront(U&& waypoint)
    {
        path_points.insert(path_points.begin(), std::forward<U>(waypoint));
    }

    /**
     * @brief Construct a new PathPoint in place at the end of the path.
     * @tparam Args Arguments to construct a PathPoint.
     * @param args Arguments to construct a PathPoint.
     */
    template <typename... Args,
              typename = typename std::enable_if_t<std::is_constructible_v<PathPoint, Args...>>>
    void EmplaceBack(Args&&... args)
    {
        path_points.emplace_back(std::forward<Args>(args)...);
    }

    template <typename... Args,
              typename = typename std::enable_if_t<std::is_constructible_v<PathPoint, Args...>>>
    void EmplaceFront(Args&&... args)
    {
        path_points.emplace(path_points.begin(), std::forward<Args>(args)...);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<PathPoint, std::decay_t<U>>>>
    void Insert(std::size_t index, U&& waypoint)
    {
        if (index > path_points.size())
        {
            throw std::out_of_range("Index out of range for PlanningPath");
        }
        path_points.insert(path_points.begin() + index, std::forward<U>(waypoint));
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<typename PathPoints::iterator, std::decay_t<U>>>>
    void Insert(U&& position, const PathPoint& waypoint)
    {
        path_points.insert(std::forward<U>(position), waypoint);
    }

    void PopBack()
    {
        if (path_points.empty())
        {
            throw std::out_of_range("PlanningPath is empty");
        }
        path_points.pop_back();
    }

    void PopFront()
    {
        if (path_points.empty())
        {
            throw std::out_of_range("PlanningPath is empty");
        }
        path_points.erase(path_points.begin());
    }

    const PathPoint& Front() const
    {
        if (path_points.empty())
        {
            throw std::out_of_range("PlanningPath is empty");
        }
        return path_points.front();
    }

    PathPoint& Front()
    {
        return const_cast<PathPoint&>(const_cast<const PlanningPath*>(this)->Front());
    }

    const PathPoint& Back() const
    {
        if (path_points.empty())
        {
            throw std::out_of_range("PlanningPath is empty");
        }
        return path_points.back();
    }

    PathPoint& Back()
    {
        return const_cast<PathPoint&>(const_cast<const PlanningPath*>(this)->Back());
    }

    PathPoint& operator[](std::size_t index)
    {
        if (index >= path_points.size())
        {
            throw std::out_of_range("Index out of range for PlanningPath");
        }
        return path_points.at(index);
    }

    const PathPoint& operator[](std::size_t index) const
    {
        if (index >= path_points.size())
        {
            throw std::out_of_range("Index out of range for PlanningPath");
        }
        return path_points.at(index);
    }

    PathPoint& At(std::size_t index)
    {
        if (index >= path_points.size())
        {
            throw std::out_of_range("Index out of range for PlanningPath");
        }
        return path_points.at(index);
    }

    const PathPoint& At(std::size_t index) const
    {
        if (index >= path_points.size())
        {
            throw std::out_of_range("Index out of range for PlanningPath");
        }
        return path_points.at(index);
    }
public:
    /**
     * @brief Sum of Euclidean segment lengths over odometry poses projected onto the XY plane.
     * @return Length of the path in 2D (XY plane).
     * @note This method computes the length by summing the distances between consecutive waypoints, projected onto the XY plane.
     */
    T GetLength2D() const noexcept
    {
        T length = static_cast<T>(0);
        if (path_points.size() < 2)
        {
            return length;
        }

        for (std::size_t i = 1; i < path_points.size(); ++i)
        {
            length += static_cast<Point2D<T>>(path_points[i - 1].GetPose()).DistOfEuclidean(path_points[i].GetPose());
        }
        return length;
    }
    /**
     * @brief Sum of Euclidean segment lengths over odometry poses in 3D space.
     * @return The total length of the path in 3D space.
     */
    T GetLength3D() const noexcept
    {
        T length = static_cast<T>(0);
        if (path_points.size() < 2)
        {
            return length;
        }

        for (std::size_t i = 1; i < path_points.size(); ++i)
        {
            length += path_points[i - 1].GetPose().DistOfEuclidean(path_points[i].GetPose());
        }
        return length;
    }

    /**
     * @brief Convert path summary to string.
     */
    std::string ToString() const
    {
        return "PlanningPath{timestamp=" + timestamp_.ToString() +
               ", frame_id=" + frame_id_ +
               ", size=" + std::to_string(path_points.size()) +
               ", length3d=" + std::to_string(GetLength3D()) + "}";
    }
};

} // namespace structure
