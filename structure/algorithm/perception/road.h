#pragma once

#include "..\common.h"
#include "lane.h"

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace structure
{
namespace perception
{

/**
 * @brief 道路语义类型。
 */
enum class RoadType : uint8_t
{
    UNKNOWN_0,            /**< 未知道路类型。 */
    CITY_STREET_1,        /**< 城市普通道路。 */
    HIGHWAY_2,            /**< 高速公路主线。 */
    RAMP_3,               /**< 进出匝道。 */
    INTERSECTION_LINK_4,  /**< 路口连接段（导流或连接车道）。 */
    PARKING_ROAD_5,       /**< 停车区域内部道路。 */
};

/**
 * @brief 车道线/边界线类型。
 */
enum class RoadBoundaryType : uint8_t
{
    UNKNOWN_0,      /**< 未知边界类型。 */
    DASHED_1,       /**< 虚线边界。 */
    SOLID_2,        /**< 单实线边界。 */
    DOUBLE_SOLID_3, /**< 双实线边界。 */
    CURB_4,         /**< 路缘石边界。 */
    WALL_5,         /**< 墙体或刚性护栏边界。 */
};

/**
 * @brief 道路主导转向属性。
 */
enum class RoadTurnType : uint8_t
{
    UNKNOWN_0,  /**< 未知转向属性。 */
    STRAIGHT_1, /**< 主导方向为直行。 */
    LEFT_2,     /**< 主导方向为左转。 */
    RIGHT_3,    /**< 主导方向为右转。 */
    UTURN_4,    /**< 主导方向为掉头。 */
};

/**
 * @brief 道路基础描述类。
 *
 * 使用中心线与左右边界线描述道路几何，附带车道数、限速、通行属性等基础信息。
 */
template <typename T>
class Road
{
public:
    using Timestamp = structure::Timestamp;
    using UUID = structure::UUID;
    using Vertex = structure::Vertex3D<T>;
    using Polyline = std::vector<Vertex>;
    using LaneData = Lane<T>;
    using LaneContainer = std::vector<LaneData>;
    using LaneIDList = std::vector<UUID>;

private:
    Timestamp timestamp_{};
    UUID id_{0};
    RoadType type_{RoadType::UNKNOWN_0};
    RoadTurnType turn_type_{RoadTurnType::UNKNOWN_0};

    Polyline centerline_{};
    Polyline left_boundary_{};
    Polyline right_boundary_{};
    RoadBoundaryType left_boundary_type_{RoadBoundaryType::UNKNOWN_0};
    RoadBoundaryType right_boundary_type_{RoadBoundaryType::UNKNOWN_0};

    std::string road_name_{};
    uint32_t lane_count_{0};
    T speed_limit_mps_{static_cast<T>(0)};
    bool is_one_way_{false};
    bool is_junction_{false};

    // 基于 lane 的拓扑与几何数据。
    LaneIDList lane_ids_{};
    LaneContainer lanes_{};

public:
    Road() = default;

    /**
     * @brief 构造道路对象。
     * @param timestamp 数据时间戳。
     * @param id 道路唯一 ID。
     * @param type 道路类型。
     * @param centerline 中心线点集。
     */
    Road(const Timestamp& timestamp,
         UUID id,
         RoadType type,
         const Polyline& centerline)
        : timestamp_(timestamp), id_(id), type_(type), centerline_(centerline)
    {
    }

public:
    const Timestamp& GetTimestamp() const noexcept
    {
        return timestamp_;
    }

    UUID GetID() const noexcept
    {
        return id_;
    }

    RoadType GetType() const noexcept
    {
        return type_;
    }

    RoadTurnType GetTurnType() const noexcept
    {
        return turn_type_;
    }

    const Polyline& GetCenterline() const noexcept
    {
        return centerline_;
    }

    Polyline& GetCenterline() noexcept
    {
        return centerline_;
    }

    const Polyline& GetLeftBoundary() const noexcept
    {
        return left_boundary_;
    }

    Polyline& GetLeftBoundary() noexcept
    {
        return left_boundary_;
    }

    const Polyline& GetRightBoundary() const noexcept
    {
        return right_boundary_;
    }

    Polyline& GetRightBoundary() noexcept
    {
        return right_boundary_;
    }

    RoadBoundaryType GetLeftBoundaryType() const noexcept
    {
        return left_boundary_type_;
    }

    RoadBoundaryType GetRightBoundaryType() const noexcept
    {
        return right_boundary_type_;
    }

    const std::string& GetRoadName() const noexcept
    {
        return road_name_;
    }

    const LaneIDList& GetLaneIDs() const noexcept
    {
        return lane_ids_;
    }

    LaneIDList& GetLaneIDs() noexcept
    {
        return lane_ids_;
    }

    const LaneContainer& GetLanes() const noexcept
    {
        return lanes_;
    }

    LaneContainer& GetLanes() noexcept
    {
        return lanes_;
    }

    uint32_t GetLaneCount() const noexcept
    {
        if (!lanes_.empty())
        {
            return static_cast<uint32_t>(lanes_.size());
        }
        if (!lane_ids_.empty())
        {
            return static_cast<uint32_t>(lane_ids_.size());
        }
        return lane_count_;
    }

    T GetSpeedLimitMps() const noexcept
    {
        return speed_limit_mps_;
    }

    bool IsOneWay() const noexcept
    {
        return is_one_way_;
    }

    bool IsJunction() const noexcept
    {
        return is_junction_;
    }

    bool HasGeometry() const noexcept
    {
        if (centerline_.size() >= 2)
        {
            return true;
        }
        for (const auto& lane : lanes_)
        {
            if (lane.HasGeometry())
            {
                return true;
            }
        }
        return false;
    }

    bool HasLanes() const noexcept
    {
        return !lanes_.empty() || !lane_ids_.empty();
    }

    bool HasLaneObjects() const noexcept
    {
        return !lanes_.empty();
    }

    bool HasLaneIDs() const noexcept
    {
        return !lane_ids_.empty();
    }

    /**
     * @brief 判断道路中心线是否可由 lane 数据重建。
     */
    bool CanBuildGeometryFromLanes() const noexcept
    {
        for (const auto& lane : lanes_)
        {
            if (lane.HasGeometry())
            {
                return true;
            }
        }
        return centerline_.size() >= 2;
    }

    /**
     * @brief 计算中心线的三维长度。
     * @return 中心线累计欧氏距离。
     */
    T GetCenterlineLength3D() const noexcept
    {
        if (centerline_.size() < 2)
        {
            for (const auto& lane : lanes_)
            {
                if (lane.HasGeometry())
                {
                    return lane.GetCenterlineLength3D();
                }
            }
        }

        T length = static_cast<T>(0);
        if (centerline_.size() < 2)
        {
            return length;
        }

        for (std::size_t i = 1; i < centerline_.size(); ++i)
        {
            length += centerline_[i - 1].DistOfEuclidean(centerline_[i]);
        }
        return length;
    }

public:
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Timestamp>>>
    void SetTimestamp(U&& timestamp) noexcept
    {
        timestamp_ = std::forward<U>(timestamp);
    }

    void SetID(UUID id = 0) noexcept
    {
        id_ = id;
    }

    void SetType(RoadType type = RoadType::UNKNOWN_0) noexcept
    {
        type_ = type;
    }

    void SetTurnType(RoadTurnType turn_type = RoadTurnType::UNKNOWN_0) noexcept
    {
        turn_type_ = turn_type;
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Polyline>>>
    void SetCenterline(U&& centerline) noexcept
    {
        centerline_ = std::forward<U>(centerline);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Polyline>>>
    void SetLeftBoundary(U&& left_boundary) noexcept
    {
        left_boundary_ = std::forward<U>(left_boundary);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Polyline>>>
    void SetRightBoundary(U&& right_boundary) noexcept
    {
        right_boundary_ = std::forward<U>(right_boundary);
    }

    void SetLeftBoundaryType(RoadBoundaryType type = RoadBoundaryType::UNKNOWN_0) noexcept
    {
        left_boundary_type_ = type;
    }

    void SetRightBoundaryType(RoadBoundaryType type = RoadBoundaryType::UNKNOWN_0) noexcept
    {
        right_boundary_type_ = type;
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    void SetRoadName(U&& road_name)
    {
        road_name_ = std::forward<U>(road_name);
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, LaneIDList>>>
    void SetLaneIDs(U&& lane_ids) noexcept
    {
        lane_ids_ = std::forward<U>(lane_ids);
        if (!lane_ids_.empty())
        {
            lane_count_ = static_cast<uint32_t>(lane_ids_.size());
        }
    }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, LaneContainer>>>
    void SetLanes(U&& lanes)
    {
        lanes_ = std::forward<U>(lanes);
        lane_ids_.clear();
        lane_ids_.reserve(lanes_.size());
        for (const auto& lane : lanes_)
        {
            lane_ids_.push_back(lane.GetID());
        }
        lane_count_ = static_cast<uint32_t>(lanes_.size());
    }

    void SetLaneCount(uint32_t lane_count) noexcept
    {
        lane_count_ = lane_count;
    }

    void SetSpeedLimitMps(T speed_limit_mps) noexcept
    {
        speed_limit_mps_ = ClampNonNegative(speed_limit_mps);
    }

    void SetOneWay(bool one_way) noexcept
    {
        is_one_way_ = one_way;
    }

    void SetJunction(bool is_junction) noexcept
    {
        is_junction_ = is_junction;
    }

    void PushCenterPoint(const Vertex& point)
    {
        centerline_.push_back(point);
    }

    void AddLaneID(UUID lane_id)
    {
        lane_ids_.push_back(lane_id);
        lane_count_ = static_cast<uint32_t>(lane_ids_.size());
    }

    void AddLane(const LaneData& lane)
    {
        lanes_.push_back(lane);
        lane_ids_.push_back(lane.GetID());
        lane_count_ = static_cast<uint32_t>(lanes_.size());
    }

    void AddLane(LaneData&& lane)
    {
        lane_ids_.push_back(lane.GetID());
        lanes_.push_back(std::move(lane));
        lane_count_ = static_cast<uint32_t>(lanes_.size());
    }

    void ClearLaneData() noexcept
    {
        lane_ids_.clear();
        lanes_.clear();
        lane_count_ = 0;
    }

    /**
     * @brief 根据已有 lane 对象同步道路几何。
     *
     * 规则：
     * - centerline 使用首个有效 lane 的中心线。
     * - left_boundary 使用首个有效 lane 的左边界。
     * - right_boundary 使用最后一个有效 lane 的右边界。
     */
    void SyncGeometryFromLanes()
    {
        if (lanes_.empty())
        {
            return;
        }

        bool has_centerline = false;
        bool has_left = false;
        bool has_right = false;

        for (const auto& lane : lanes_)
        {
            if (!has_centerline && lane.HasGeometry())
            {
                centerline_ = lane.GetCenterline();
                has_centerline = true;
            }
            if (!has_left && !lane.GetLeftBoundary().empty())
            {
                left_boundary_ = lane.GetLeftBoundary();
                left_boundary_type_ = ConvertBoundaryType(lane.GetLeftBoundaryType());
                has_left = true;
            }
        }

        for (auto it = lanes_.rbegin(); it != lanes_.rend(); ++it)
        {
            if (!has_right && !it->GetRightBoundary().empty())
            {
                right_boundary_ = it->GetRightBoundary();
                right_boundary_type_ = ConvertBoundaryType(it->GetRightBoundaryType());
                has_right = true;
                break;
            }
        }
    }

private:
    static RoadBoundaryType ConvertBoundaryType(LaneBoundaryType lane_boundary_type) noexcept
    {
        switch (lane_boundary_type)
        {
        case LaneBoundaryType::DASHED_1:
            return RoadBoundaryType::DASHED_1;
        case LaneBoundaryType::SOLID_2:
            return RoadBoundaryType::SOLID_2;
        case LaneBoundaryType::DOUBLE_SOLID_3:
            return RoadBoundaryType::DOUBLE_SOLID_3;
        case LaneBoundaryType::CURB_4:
            return RoadBoundaryType::CURB_4;
        case LaneBoundaryType::VIRTUAL_5:
        case LaneBoundaryType::UNKNOWN_0:
        default:
            return RoadBoundaryType::UNKNOWN_0;
        }
    }

    static T ClampNonNegative(T value) noexcept
    {
        if (value < static_cast<T>(0))
        {
            return static_cast<T>(0);
        }
        return value;
    }
};

} // namespace perception
} // namespace structure
