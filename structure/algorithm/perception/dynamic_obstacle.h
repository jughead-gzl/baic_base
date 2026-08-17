#pragma once

#include "obstacle.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace structure
{
namespace perception
{

/**
 * @brief 动态障碍物行为意图类型。
 */
enum class DynamicIntentType : uint8_t
{
    UNKNOWN_0,
    KEEP_LANE_1,
    CUT_IN_2,
    CROSSING_3,
    STOPPING_4,
};

template<typename T>
class DynamicObstacle : public Obstacle<T>
{
public:
    /** @brief 基类类型别名。 */
    using Base = Obstacle<T>;
    /** @brief 预测轨迹类型，使用路径点序列表示。 */
    using PredictPath = std::vector<structure::PathPoint<T>>;
    /** @brief 里程计状态类型。 */
    using Odometry = typename Base::Odometry;
    /** @brief 时间戳类型。 */
    using Timestamp = typename Base::Timestamp;
    /** @brief 唯一标识类型。 */
    using UUID = typename Base::UUID;

private:
    /** @brief 预测轨迹点序列。 */
    PredictPath predict_path_;

    /** @brief 预测时间范围，单位秒。 */
    T predict_horizon_s_{static_cast<T>(0)};
    /** @brief 预测轨迹时间间隔，单位秒。 */
    T predict_dt_s_{static_cast<T>(0)};
    /** @brief 预测结果置信度，建议范围 [0, 1]。 */
    T predict_confidence_{static_cast<T>(0)};

    /** @brief 运动学最大速度约束。 */
    T max_speed_{static_cast<T>(0)};
    /** @brief 运动学最大加速度约束。 */
    T max_accel_{static_cast<T>(0)};
    /** @brief 偏航角速度上限约束。 */
    T yaw_rate_limit_{static_cast<T>(0)};

    /** @brief 当前行为意图。 */
    DynamicIntentType intent_{DynamicIntentType::UNKNOWN_0};
    /** @brief 行为意图置信度，建议范围 [0, 1]。 */
    T intent_confidence_{static_cast<T>(0)};
    /** @brief 轨迹来源标识（如模型名、模块名）。 */
    std::string trajectory_source_{};
    /** @brief 轨迹年龄（自生成以来的计数）。 */
    uint32_t trajectory_age_{0};
    /** @brief 轨迹是否已过期。 */
    bool is_prediction_stale_{false};

public:
    /** @brief 默认构造函数，自动将运动类型标记为动态。 */
    DynamicObstacle()
    {
        this->SetMotion(ObstacleMotionType::DYNAMIC_2);
    }

    /**
     * @brief 构造动态障碍物对象。
     * @param timestamp 状态时间戳。
     * @param id 目标唯一 ID。
     * @param type 目标语义类型。
     * @param odometry 目标里程计状态。
     * @param predict_path 初始预测轨迹。
     */
    DynamicObstacle(const Timestamp& timestamp,
                    UUID id,
                    ObstacleType type,
                    const Odometry& odometry,
                    PredictPath predict_path = PredictPath{})
        : Base(timestamp, id, type, ObstacleMotionType::DYNAMIC_2, odometry), predict_path_(std::move(predict_path))
    {
    }

    /** @brief 获取预测轨迹（只读）。 */
    /** @return 预测轨迹常量引用。 */
    const PredictPath& GetPredictPath() const noexcept
    {
        return predict_path_;
    }

    /** @brief 获取预测轨迹（可写）。 */
    /** @return 预测轨迹可写引用。 */
    PredictPath& GetPredictPath() noexcept
    {
        return predict_path_;
    }

    /** @brief 判断是否存在预测轨迹。 */
    /** @return 非空返回 true。 */
    bool HasPredictPath() const noexcept
    {
        return !predict_path_.empty();
    }

    /** @brief 获取预测轨迹点数量。 */
    /** @return 轨迹点数量。 */
    std::size_t PredictPathSize() const noexcept
    {
        return predict_path_.size();
    }

    /** @brief 清空预测轨迹。 */
    void ClearPredictPath() noexcept
    {
        predict_path_.clear();
    }

    /** @brief 获取预测时间范围。 */
    /** @return 预测范围（秒）。 */
    T GetPredictHorizonSeconds() const noexcept
    {
        return predict_horizon_s_;
    }

    /** @brief 获取预测时间步长。 */
    /** @return 预测步长（秒）。 */
    T GetPredictDeltaTimeSeconds() const noexcept
    {
        return predict_dt_s_;
    }

    /** @brief 获取预测置信度。 */
    /** @return 预测置信度。 */
    T GetPredictConfidence() const noexcept
    {
        return predict_confidence_;
    }

    /** @brief 获取最大速度约束。 */
    /** @return 最大速度。 */
    T GetMaxSpeed() const noexcept
    {
        return max_speed_;
    }

    /** @brief 获取最大加速度约束。 */
    /** @return 最大加速度。 */
    T GetMaxAccel() const noexcept
    {
        return max_accel_;
    }

    /** @brief 获取偏航角速度上限。 */
    /** @return 偏航角速度上限。 */
    T GetYawRateLimit() const noexcept
    {
        return yaw_rate_limit_;
    }

    /** @brief 获取行为意图类型。 */
    /** @return 动态意图枚举。 */
    DynamicIntentType GetIntent() const noexcept
    {
        return intent_;
    }

    /** @brief 获取行为意图置信度。 */
    /** @return 意图置信度。 */
    T GetIntentConfidence() const noexcept
    {
        return intent_confidence_;
    }

    /** @brief 获取轨迹来源。 */
    /** @return 轨迹来源字符串常量引用。 */
    const std::string& GetTrajectorySource() const noexcept
    {
        return trajectory_source_;
    }

    /** @brief 获取轨迹年龄。 */
    /** @return 轨迹年龄计数。 */
    uint32_t GetTrajectoryAge() const noexcept
    {
        return trajectory_age_;
    }

    /** @brief 判断预测轨迹是否过期。 */
    /** @return 过期返回 true。 */
    bool IsPredictionStale() const noexcept
    {
        return is_prediction_stale_;
    }

    /** @brief 判断是否包含轨迹来源信息。 */
    /** @return 来源字符串非空返回 true。 */
    bool HasTrajectorySource() const noexcept
    {
        return !trajectory_source_.empty();
    }

public:
    /**
     * @brief 设置预测轨迹。
     * @tparam U 轨迹类型，需与 PredictPath 等价。
     * @param path 新预测轨迹。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, PredictPath>>>
    void SetPredictPath(U&& path) noexcept
    {
        predict_path_ = std::forward<U>(path);
    }

    /**
     * @brief 设置预测时间范围。
     * @param horizon_seconds 预测范围（秒），负值会被裁剪为 0。
     */
    void SetPredictHorizonSeconds(T horizon_seconds) noexcept
    {
        predict_horizon_s_ = ClampNonNegative(horizon_seconds);
    }

    /**
     * @brief 设置预测时间步长。
     * @param dt_seconds 时间步长（秒），负值会被裁剪为 0。
     */
    void SetPredictDeltaTimeSeconds(T dt_seconds) noexcept
    {
        predict_dt_s_ = ClampNonNegative(dt_seconds);
    }

    /**
     * @brief 设置预测置信度。
     * @param confidence 输入置信度，会被裁剪到 [0, 1]。
     */
    void SetPredictConfidence(T confidence) noexcept
    {
        predict_confidence_ = ClampUnit(confidence);
    }

    /**
     * @brief 设置最大速度约束。
     * @param max_speed 最大速度，负值会被裁剪为 0。
     */
    void SetMaxSpeed(T max_speed) noexcept
    {
        max_speed_ = ClampNonNegative(max_speed);
    }

    /**
     * @brief 设置最大加速度约束。
     * @param max_accel 最大加速度，负值会被裁剪为 0。
     */
    void SetMaxAccel(T max_accel) noexcept
    {
        max_accel_ = ClampNonNegative(max_accel);
    }

    /**
     * @brief 设置偏航角速度上限。
     * @param yaw_rate_limit 偏航角速度上限，负值会被裁剪为 0。
     */
    void SetYawRateLimit(T yaw_rate_limit) noexcept
    {
        yaw_rate_limit_ = ClampNonNegative(yaw_rate_limit);
    }

    /**
     * @brief 设置行为意图。
     * @param intent 新意图类型。
     */
    void SetIntent(DynamicIntentType intent) noexcept
    {
        intent_ = intent;
    }

    /**
     * @brief 设置意图置信度。
     * @param confidence 输入置信度，会被裁剪到 [0, 1]。
     */
    void SetIntentConfidence(T confidence) noexcept
    {
        intent_confidence_ = ClampUnit(confidence);
    }

    /**
     * @brief 设置轨迹来源。
     * @tparam U 字符串类型，需与 std::string 等价。
     * @param trajectory_source 轨迹来源描述。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, std::string>>>
    void SetTrajectorySource(U&& trajectory_source)
    {
        trajectory_source_ = std::forward<U>(trajectory_source);
    }

    /**
     * @brief 设置轨迹年龄。
     * @param age 新轨迹年龄。
     */
    void SetTrajectoryAge(uint32_t age) noexcept
    {
        trajectory_age_ = age;
    }

    /** @brief 轨迹年龄自增 1。 */
    void IncreaseTrajectoryAge() noexcept
    {
        ++trajectory_age_;
    }

    /**
     * @brief 设置轨迹是否过期。
     * @param stale 过期标记。
     */
    void SetPredictionStale(bool stale) noexcept
    {
        is_prediction_stale_ = stale;
    }

private:
    /**
     * @brief 将值裁剪到 [0, 1]。
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
     * @brief 将值裁剪为非负。
     * @param value 输入值。
     * @return 若小于 0 则返回 0，否则返回原值。
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

};
};