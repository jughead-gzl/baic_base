#pragma once

#include "..\common.h"

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
 * @brief 车道类型。
 */
enum class LaneType : uint8_t
{
    UNKNOWN_0,      /**< 未知车道。 */
    DRIVING_1,      /**< 普通行驶车道。 */
    EMERGENCY_2,    /**< 应急车道。 */
    ACCEL_3,        /**< 加速车道。 */
    DECEL_4,        /**< 减速车道。 */
    TURN_LEFT_5,    /**< 左转专用车道。 */
    TURN_RIGHT_6,   /**< 右转专用车道。 */
    BUS_ONLY_7,     /**< 公交专用车道。 */
    BICYCLE_8,      /**< 非机动车道。 */
    PARKING_9,      /**< 停车车道/停车带。 */
};

/**
 * @brief 车道边界类型。
 */
enum class LaneBoundaryType : uint8_t
{
    UNKNOWN_0,        /**< 未知边界。 */
    DASHED_1,         /**< 虚线边界。 */
    SOLID_2,          /**< 实线边界。 */
    DOUBLE_SOLID_3,   /**< 双实线边界。 */
    CURB_4,           /**< 路缘边界。 */
    VIRTUAL_5,        /**< 虚拟边界（地图推断）。 */
};

/**
 * @brief 车道通行方向。
 */
enum class LaneDirection : uint8_t
{
    UNKNOWN_0,         /**< 未知方向。 */
    FORWARD_1,         /**< 顺向单向。 */
    BACKWARD_2,        /**< 逆向单向。 */
    BIDIRECTIONAL_3,   /**< 双向通行。 */
};

/**
 * @brief 车道主导转向属性。
 */
enum class LaneTurnType : uint8_t
{
    UNKNOWN_0,   /**< 未知转向。 */
    STRAIGHT_1,  /**< 直行。 */
    LEFT_2,      /**< 左转。 */
    RIGHT_3,     /**< 右转。 */
    UTURN_4,     /**< 掉头。 */
};

/**
 * @brief 车道基础描述类。
 *
 * 使用中心线和左右边界表达几何，保留限速、宽度、方向、连通关系等属性。
 */
template <typename T>
class Lane
{
public:
    using Timestamp = structure::Timestamp;
    using UUID = structure::UUID;
    using Vertex = structure::Vertex3D<T>;
    using Polyline = std::vector<Vertex>;

private:
    Timestamp timestamp_{};
    UUID id_{0};
    UUID road_id_{0};

    LaneType type_{LaneType::UNKNOWN_0};
    LaneDirection direction_{LaneDirection::UNKNOWN_0};
    LaneTurnType turn_type_{LaneTurnType::UNKNOWN_0};

    Polyline centerline_{};
    Polyline left_boundary_{};
    Polyline right_boundary_{};
    LaneBoundaryType left_boundary_type_{LaneBoundaryType::UNKNOWN_0};
    LaneBoundaryType right_boundary_type_{LaneBoundaryType::UNKNOWN_0};

    T width_m_{static_cast<T>(0)};
    T speed_limit_mps_{static_cast<T>(0)};
    bool is_drivable_{true};
    bool is_junction_lane_{false};

    UUID left_neighbor_lane_id_{0};
    UUID right_neighbor_lane_id_{0};
    std::vector<UUID> predecessor_lane_ids_{};
    std::vector<UUID> successor_lane_ids_{};

    std::string lane_name_{};

public:
    Lane() = default;

    /**
     * @brief 构造车道对象。
     * @param timestamp 数据时间戳。
     * @param id 车道唯一 ID。
     * @param road_id 所属道路 ID。
     * @param type 车道类型。
     * @param centerline 车道中心线。
     */
    Lane(const Timestamp& timestamp,
         UUID id,
         UUID road_id,
         LaneType type,
         const Polyline& centerline)
        : timestamp_(timestamp),
          id_(id),
          road_id_(road_id),
          type_(type),
          centerline_(centerline)
    {
    }

public:
    const Timestamp& GetTimestamp() const noexcept { return timestamp_; }
    UUID GetID() const noexcept { return id_; }
    UUID GetRoadID() const noexcept { return road_id_; }

    LaneType GetType() const noexcept { return type_; }
    LaneDirection GetDirection() const noexcept { return direction_; }
    LaneTurnType GetTurnType() const noexcept { return turn_type_; }

    const Polyline& GetCenterline() const noexcept { return centerline_; }
    Polyline& GetCenterline() noexcept { return centerline_; }

    const Polyline& GetLeftBoundary() const noexcept { return left_boundary_; }
    Polyline& GetLeftBoundary() noexcept { return left_boundary_; }

    const Polyline& GetRightBoundary() const noexcept { return right_boundary_; }
    Polyline& GetRightBoundary() noexcept { return right_boundary_; }

    LaneBoundaryType GetLeftBoundaryType() const noexcept { return left_boundary_type_; }
    LaneBoundaryType GetRightBoundaryType() const noexcept { return right_boundary_type_; }

    T GetWidthMeters() const noexcept { return width_m_; }
    T GetSpeedLimitMps() const noexcept { return speed_limit_mps_; }

    bool IsDrivable() const noexcept { return is_drivable_; }
    bool IsJunctionLane() const noexcept { return is_junction_lane_; }

    UUID GetLeftNeighborLaneID() const noexcept { return left_neighbor_lane_id_; }
    UUID GetRightNeighborLaneID() const noexcept { return right_neighbor_lane_id_; }

    const std::vector<UUID>& GetPredecessorLaneIDs() const noexcept { return predecessor_lane_ids_; }
    std::vector<UUID>& GetPredecessorLaneIDs() noexcept { return predecessor_lane_ids_; }

    const std::vector<UUID>& GetSuccessorLaneIDs() const noexcept { return successor_lane_ids_; }
    std::vector<UUID>& GetSuccessorLaneIDs() noexcept { return successor_lane_ids_; }

    const std::string& GetLaneName() const noexcept { return lane_name_; }

    bool HasGeometry() const noexcept { return centerline_.size() >= 2; }

    /**
     * @brief 计算中心线三维长度。
     * @return 累计欧氏长度。
     */
    T GetCenterlineLength3D() const noexcept
    {
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
    void SetTimestamp(U&& timestamp) noexcept { timestamp_ = std::forward<U>(timestamp); }

    void SetID(UUID id = 0) noexcept { id_ = id; }
    void SetRoadID(UUID road_id = 0) noexcept { road_id_ = road_id; }

    void SetType(LaneType type = LaneType::UNKNOWN_0) noexcept { type_ = type; }
    void SetDirection(LaneDirection direction = LaneDirection::UNKNOWN_0) noexcept { direction_ = direction; }
    void SetTurnType(LaneTurnType turn_type = LaneTurnType::UNKNOWN_0) noexcept { turn_type_ = turn_type; }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Polyline>>>
    void SetCenterline(U&& centerline) noexcept { centerline_ = std::forward<U>(centerline); }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Polyline>>>
    void SetLeftBoundary(U&& left_boundary) noexcept { left_boundary_ = std::forward<U>(left_boundary); }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Polyline>>>
    void SetRightBoundary(U&& right_boundary) noexcept { right_boundary_ = std::forward<U>(right_boundary); }

    void SetLeftBoundaryType(LaneBoundaryType type = LaneBoundaryType::UNKNOWN_0) noexcept
    {
        left_boundary_type_ = type;
    }

    void SetRightBoundaryType(LaneBoundaryType type = LaneBoundaryType::UNKNOWN_0) noexcept
    {
        right_boundary_type_ = type;
    }

    void SetWidthMeters(T width_m) noexcept { width_m_ = ClampNonNegative(width_m); }
    void SetSpeedLimitMps(T speed_limit_mps) noexcept { speed_limit_mps_ = ClampNonNegative(speed_limit_mps); }

    void SetDrivable(bool drivable) noexcept { is_drivable_ = drivable; }
    void SetJunctionLane(bool is_junction_lane) noexcept { is_junction_lane_ = is_junction_lane; }

    void SetLeftNeighborLaneID(UUID lane_id) noexcept { left_neighbor_lane_id_ = lane_id; }
    void SetRightNeighborLaneID(UUID lane_id) noexcept { right_neighbor_lane_id_ = lane_id; }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::vector<UUID>>>>
    void SetPredecessorLaneIDs(U&& lane_ids) noexcept { predecessor_lane_ids_ = std::forward<U>(lane_ids); }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::vector<UUID>>>>
    void SetSuccessorLaneIDs(U&& lane_ids) noexcept { successor_lane_ids_ = std::forward<U>(lane_ids); }

    void AddPredecessorLaneID(UUID lane_id) { predecessor_lane_ids_.push_back(lane_id); }
    void AddSuccessorLaneID(UUID lane_id) { successor_lane_ids_.push_back(lane_id); }

    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    void SetLaneName(U&& lane_name) { lane_name_ = std::forward<U>(lane_name); }

    void PushCenterPoint(const Vertex& point) { centerline_.push_back(point); }
    void PushLeftBoundaryPoint(const Vertex& point) { left_boundary_.push_back(point); }
    void PushRightBoundaryPoint(const Vertex& point) { right_boundary_.push_back(point); }

private:
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
