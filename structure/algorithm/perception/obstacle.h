#pragma once

#include "..\common.h"
#include "..\alam\odometry.h"

#include <array>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

namespace structure
{
namespace perception
{
/**
 * @brief 障碍物语义类型。
 */
enum class ObstacleType : uint8_t
{
    UNKNOWN_0,
    VEHICLE_1,
    PEDESTRIAN_2,
    CYCLIST_3,
    MOTORCYCLIST_4,
    TRUCK_5,
    BUS_6,
    TRAFFIC_CONE_7,
    BARRIER_8,
    CURB_9,
    ANIMAL_10,
    DEBRIS_11,
};

/**
 * @brief 障碍物运动状态。
 */
enum class ObstacleMotionType : uint8_t
{
    UNKNOWN_0,
    STATIC_1,
    DYNAMIC_2,
};

/**
 * @brief 跟踪生命周期状态。
 */
enum class ObstacleTrackStatusType : uint8_t
{
    NEW_0,
    TRACKED_1,
    LOST_2,
    DEAD_3,
};

/**
 * @brief 障碍物几何外形类型。
 */
enum class ObstacleShapeType : uint8_t
{
    UNKNOWN_0,
    BOX_1,
    CYLINDER_2,
    POLYGON_3,
};

template <typename T>
class Obstacle
{
public:
    /** @brief 里程计状态类型。 */
    using Odometry = structure::Odometry<T>;
    /** @brief 时间戳类型。 */
    using Timestamp = structure::Timestamp;
    /** @brief 唯一标识类型。 */
    using UUID = structure::UUID;
    /** @brief 6x6 协方差矩阵的行优先展开存储。 */
    using Covariance6x6 = std::array<T, 36>;

private:
    /** @brief 目标状态时间戳。 */
    Timestamp timestamp_{};
    /** @brief 目标唯一 ID。 */
    UUID id_{0};
    /** @brief 目标语义类别。 */
    ObstacleType type_{ObstacleType::UNKNOWN_0};
    /** @brief 目标运动类型（静态/动态/未知）。 */
    ObstacleMotionType motion_{ObstacleMotionType::UNKNOWN_0};
    /** @brief 目标运动学状态（位姿、速度、加速度）。 */
    Odometry odometry_{};

    /** @brief 检测模块输出的置信度，取值范围建议为 [0, 1]。 */
    T detection_confidence_{static_cast<T>(0)};
    /** @brief 跟踪模块输出的置信度，取值范围建议为 [0, 1]。 */
    T tracking_confidence_{static_cast<T>(0)};
    /** @brief 跟踪生命周期状态。 */
    ObstacleTrackStatusType track_status_{ObstacleTrackStatusType::NEW_0};
    /** @brief 目标连续存活帧数。 */
    uint32_t track_age_{0};
    /** @brief 目标连续丢失帧数。 */
    uint32_t miss_count_{0};
    /** @brief 目标包围盒长度，通常单位为米。 */
    T length_{static_cast<T>(0)};
    /** @brief 目标包围盒宽度，通常单位为米。 */
    T width_{static_cast<T>(0)};
    /** @brief 目标包围盒高度，通常单位为米。 */
    T height_{static_cast<T>(0)};

    /** @brief 位姿协方差矩阵。 */
    Covariance6x6 pose_cov_{};
    /** @brief 速度协方差矩阵。 */
    Covariance6x6 twist_cov_{};
    /** @brief 数据来源传感器名称或标识。 */
    std::string sensor_source_{};
    /** @brief 原始测量时间戳。 */
    Timestamp measurement_time_{};
    /** @brief 当前对象最近一次更新时间戳。 */
    Timestamp update_time_{};
    /** @brief 更细粒度语义标签（如 sedan、adult）。 */
    std::string subtype_{};
    /** @brief 几何外形类型。 */
    ObstacleShapeType shape_type_{ObstacleShapeType::UNKNOWN_0};

public:
    /** @brief 默认构造函数。 */
    Obstacle() = default;

    /**
     * @brief 构造一个障碍物基础对象。
     * @param timestamp 目标状态时间戳。
     * @param id 目标唯一 ID。
     * @param type 目标语义类型。
     * @param motion 目标运动类型。
     * @param odometry 目标里程计状态。
     */
    Obstacle(const Timestamp& timestamp,
             UUID id,
             ObstacleType type,
             ObstacleMotionType motion,
             const Odometry& odometry)
        : timestamp_(timestamp), id_(id), type_(type), motion_(motion), odometry_(odometry)
    {
    }

    /** @brief 虚析构函数。 */
    virtual ~Obstacle() = default;

public:
    /** @brief 获取状态时间戳。 */
    /** @return 时间戳只读引用。 */
    const Timestamp& GetTimestamp() const noexcept
    {
        return timestamp_;
    }

    /** @brief 获取目标 ID。 */
    /** @return 目标唯一标识。 */
    UUID GetID() const noexcept
    {
        return id_;
    }

    /** @brief 获取目标语义类型。 */
    /** @return 语义类型枚举。 */
    ObstacleType GetType() const noexcept
    {
        return type_;
    }

    /** @brief 获取里程计状态（只读）。 */
    /** @return 里程计状态只读引用。 */
    const Odometry& GetOdometry() const noexcept
    {
        return odometry_;
    }

    /** @brief 获取里程计状态（可写）。 */
    /** @return 里程计状态可写引用。 */
    Odometry& GetOdometry() noexcept
    {
        return odometry_;
    }

    /** @brief 获取运动类型。 */
    /** @return 运动类型枚举。 */
    ObstacleMotionType GetMotionType() const noexcept
    {
        return motion_;
    }

    /** @brief 获取检测置信度。 */
    /** @return 检测置信度。 */
    T GetDetectionConfidence() const noexcept
    {
        return detection_confidence_;
    }

    /** @brief 获取跟踪置信度。 */
    /** @return 跟踪置信度。 */
    T GetTrackingConfidence() const noexcept
    {
        return tracking_confidence_;
    }

    /** @brief 获取跟踪状态。 */
    /** @return 跟踪生命周期状态枚举。 */
    ObstacleTrackStatusType GetTrackStatus() const noexcept
    {
        return track_status_;
    }

    /** @brief 获取目标存活帧数。 */
    /** @return 存活帧数。 */
    uint32_t GetTrackAge() const noexcept
    {
        return track_age_;
    }

    /** @brief 获取连续丢失帧数。 */
    /** @return 丢失帧数。 */
    uint32_t GetMissCount() const noexcept
    {
        return miss_count_;
    }

    /** @brief 获取目标长度。 */
    /** @return 长度值。 */
    T GetLength() const noexcept
    {
        return length_;
    }

    /** @brief 获取目标宽度。 */
    /** @return 宽度值。 */
    T GetWidth() const noexcept
    {
        return width_;
    }

    /** @brief 获取目标高度。 */
    /** @return 高度值。 */
    T GetHeight() const noexcept
    {
        return height_;
    }

    /** @brief 获取位姿协方差。 */
    /** @return 6x6 位姿协方差只读引用。 */
    const Covariance6x6& GetPoseCovariance() const noexcept
    {
        return pose_cov_;
    }

    /** @brief 获取速度协方差。 */
    /** @return 6x6 速度协方差只读引用。 */
    const Covariance6x6& GetTwistCovariance() const noexcept
    {
        return twist_cov_;
    }

    /** @brief 获取传感器来源标识。 */
    /** @return 来源字符串只读引用。 */
    const std::string& GetSensorSource() const noexcept
    {
        return sensor_source_;
    }

    /** @brief 获取原始测量时间。 */
    /** @return 测量时间戳只读引用。 */
    const Timestamp& GetMeasurementTime() const noexcept
    {
        return measurement_time_;
    }

    /** @brief 获取最近更新时间。 */
    /** @return 更新时间戳只读引用。 */
    const Timestamp& GetUpdateTime() const noexcept
    {
        return update_time_;
    }

    /** @brief 获取细分类标签。 */
    /** @return 细分类字符串只读引用。 */
    const std::string& GetSubtype() const noexcept
    {
        return subtype_;
    }

    /** @brief 获取几何外形类型。 */
    /** @return 外形类型枚举。 */
    ObstacleShapeType GetShapeType() const noexcept
    {
        return shape_type_;
    }

    /** @brief 判断是否为动态目标。 */
    /** @return 若为动态返回 true，否则返回 false。 */
    bool IsDynamic() const noexcept
    {
        return motion_ == ObstacleMotionType::DYNAMIC_2;
    }

    /** @brief 判断是否为静态目标。 */
    /** @return 若为静态返回 true，否则返回 false。 */
    bool IsStatic() const noexcept
    {
        return motion_ == ObstacleMotionType::STATIC_1;
    }

    /** @brief 判断运动类型是否已知。 */
    /** @return 若非 UNKNOWN 返回 true。 */
    bool IsMotionKnown() const noexcept
    {
        return motion_ != ObstacleMotionType::UNKNOWN_0;
    }

    /** @brief 判断语义类型是否已知。 */
    /** @return 若非 UNKNOWN 返回 true。 */
    bool IsTypeKnown() const noexcept
    {
        return type_ != ObstacleType::UNKNOWN_0;
    }

    /** @brief 判断尺寸是否均为正值。 */
    /** @return 长宽高都大于 0 时返回 true。 */
    bool HasValidSize() const noexcept
    {
        return length_ > static_cast<T>(0) &&
               width_ > static_cast<T>(0) &&
               height_ > static_cast<T>(0);
    }

    /** @brief 判断是否包含来源信息。 */
    /** @return source 非空返回 true。 */
    bool HasSensorSource() const noexcept
    {
        return !sensor_source_.empty();
    }

    /** @brief 判断是否包含细分类标签。 */
    /** @return subtype 非空返回 true。 */
    bool HasSubtype() const noexcept
    {
        return !subtype_.empty();
    }

public:
    /**
     * @brief 设置状态时间戳。
     * @tparam U 时间戳类型，需与 Timestamp 等价。
     * @param timestamp 新时间戳。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Timestamp>>>
    void SetTimestamp(U&& timestamp) noexcept
    {
        timestamp_ = std::forward<U>(timestamp);
    }

    /**
     * @brief 设置目标 ID。
     * @param id 新目标 ID。
     */
    void SetID(UUID id = 0) noexcept
    {
        id_ = id;
    }

    /**
     * @brief 设置语义类型。
     * @param type 新语义类型。
     */
    void SetType(ObstacleType type = ObstacleType::UNKNOWN_0) noexcept
    {
        type_ = type;
    }

    /**
     * @brief 设置里程计状态。
     * @tparam U 状态类型，需与 Odometry 等价。
     * @param odometry 新里程计状态。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Odometry>>>
    void SetOdometry(U&& odometry) noexcept
    {
        odometry_ = std::forward<U>(odometry);
    }

    /**
     * @brief 设置检测置信度。
     * @param confidence 输入置信度，会被裁剪到 [0, 1]。
     */
    void SetDetectionConfidence(T confidence) noexcept
    {
        detection_confidence_ = ClampUnit(confidence);
    }

    /**
     * @brief 设置跟踪置信度。
     * @param confidence 输入置信度，会被裁剪到 [0, 1]。
     */
    void SetTrackingConfidence(T confidence) noexcept
    {
        tracking_confidence_ = ClampUnit(confidence);
    }

    /**
     * @brief 设置跟踪状态。
     * @param status 新跟踪状态。
     */
    void SetTrackStatus(ObstacleTrackStatusType status = ObstacleTrackStatusType::NEW_0) noexcept
    {
        track_status_ = status;
    }

    /**
     * @brief 设置存活帧数。
     * @param age 新存活帧数。
     */
    void SetTrackAge(uint32_t age = 0) noexcept
    {
        track_age_ = age;
    }

    /** @brief 存活帧数自增 1。 */
    void IncreaseTrackAge() noexcept
    {
        ++track_age_;
    }

    /**
     * @brief 设置丢失帧数。
     * @param miss_count 新丢失帧数。
     */
    void SetMissCount(uint32_t miss_count = 0) noexcept
    {
        miss_count_ = miss_count;
    }

    /** @brief 丢失帧数自增 1。 */
    void IncreaseMissCount() noexcept
    {
        ++miss_count_;
    }

    /** @brief 将丢失帧数清零。 */
    void ResetMissCount() noexcept
    {
        miss_count_ = 0;
    }

    /**
     * @brief 设置目标尺寸。
     * @param length 目标长度。
     * @param width 目标宽度。
     * @param height 目标高度。
     */
    void SetSize(T length, T width, T height) noexcept
    {
        // 对非法负尺寸做裁剪，避免传播无效几何数据。
        length_ = ClampNonNegative(length);
        width_ = ClampNonNegative(width);
        height_ = ClampNonNegative(height);
    }

    /**
     * @brief 设置位姿协方差。
     * @tparam U 协方差类型，需与 Covariance6x6 等价。
     * @param pose_cov 位姿协方差。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Covariance6x6>>>
    void SetPoseCovariance(U&& pose_cov) noexcept
    {
        pose_cov_ = std::forward<U>(pose_cov);
    }

    /**
     * @brief 设置速度协方差。
     * @tparam U 协方差类型，需与 Covariance6x6 等价。
     * @param twist_cov 速度协方差。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Covariance6x6>>>
    void SetTwistCovariance(U&& twist_cov) noexcept
    {
        twist_cov_ = std::forward<U>(twist_cov);
    }

    /**
     * @brief 设置数据来源。
     * @tparam U 字符串类型，需与 std::string 等价。
     * @param sensor_source 来源标识。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    void SetSensorSource(U&& sensor_source)
    {
        sensor_source_ = std::forward<U>(sensor_source);
    }

    /**
     * @brief 设置测量时间。
     * @tparam U 时间戳类型，需与 Timestamp 等价。
     * @param measurement_time 测量时间戳。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Timestamp>>>
    void SetMeasurementTime(U&& measurement_time) noexcept
    {
        measurement_time_ = std::forward<U>(measurement_time);
    }

    /**
     * @brief 设置更新时间。
     * @tparam U 时间戳类型，需与 Timestamp 等价。
     * @param update_time 更新时间戳。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Timestamp>>>
    void SetUpdateTime(U&& update_time) noexcept
    {
        update_time_ = std::forward<U>(update_time);
    }

    /**
     * @brief 设置细分类标签。
     * @tparam U 字符串类型，需与 std::string 等价。
     * @param subtype 细分类名称。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    void SetSubtype(U&& subtype)
    {
        subtype_ = std::forward<U>(subtype);
    }

    /**
     * @brief 设置外形类型。
     * @param shape_type 外形类型枚举。
     */
    void SetShapeType(ObstacleShapeType shape_type) noexcept
    {
        shape_type_ = shape_type;
    }

    /**
     * @brief 同时更新时间戳与里程计状态。
     * @tparam T1 时间戳类型，需与 Timestamp 等价。
     * @tparam T2 状态类型，需与 Odometry 等价。
     * @param timestamp 新时间戳。
     * @param odometry 新里程计状态。
     */
    template <typename T1, typename T2,
              typename = typename std::enable_if_t<std::is_same_v<std::decay_t<T1>, Timestamp> &&
                                                   std::is_same_v<std::decay_t<T2>, Odometry>>>
    void UpdateState(T1&& timestamp, T2&& odometry) noexcept
    {
        timestamp_ = std::forward<T1>(timestamp);
        odometry_ = std::forward<T2>(odometry);
    }

protected:
    /**
     * @brief 仅允许派生类约束运动属性，避免外部误写动静态标签。
     * @param motion 目标运动类型。
     */
    void SetMotion(ObstacleMotionType motion) noexcept
    {
        motion_ = motion;
    }

private:
    /**
     * @brief 将数值裁剪到 [0, 1]。
     * @param value 输入值。
     * @return 裁剪后的值。
     */
    static T ClampUnit(T value) noexcept
    {
        if (value < static_cast<T>(0))
        {
            return static_cast<T>(0);
        }
        if (value > static_cast<T>(1))
        {
            return static_cast<T>(1);
        }
        return value;
    }

    /**
     * @brief 将数值裁剪为非负。
     * @param value 输入值。
     * @return 若输入小于 0 则返回 0，否则返回原值。
     */
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
