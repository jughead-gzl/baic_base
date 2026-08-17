#pragma once

#include "obstacle.h"

#include <cstdint>

namespace structure
{
namespace perception
{

template<typename T>
class StaticObstacle : public Obstacle<T>
{
public:
	/** @brief 基类类型别名。 */
	using Base = Obstacle<T>;
	/** @brief 里程计状态类型。 */
	using Odometry = typename Base::Odometry;
	/** @brief 时间戳类型。 */
	using Timestamp = typename Base::Timestamp;
	/** @brief 唯一标识类型。 */
	using UUID = typename Base::UUID;

private:
	/** @brief 对应地图对象 ID。 */
	UUID map_object_id_{0};
	/** @brief 目标所在车道 ID。 */
	UUID lane_id_{0};
	/** @brief 关联道路边界 ID。 */
	UUID road_edge_id_{0};

	/** @brief 是否可通行。 */
	bool is_passable_{false};
	/** @brief 是否临时障碍（如临时施工物）。 */
	bool is_temporary_{false};
	/** @brief 静态属性置信度，建议范围 [0, 1]。 */
	T static_confidence_{static_cast<T>(0)};
	/** @brief 最近连续观测时长，单位秒。 */
	T last_observed_duration_s_{static_cast<T>(0)};

public:
	/** @brief 默认构造函数，自动将运动类型标记为静态。 */
	StaticObstacle()
	{
		this->SetMotion(ObstacleMotionType::STATIC_1);
	}

	/**
	 * @brief 构造静态障碍物对象。
	 * @param timestamp 状态时间戳。
	 * @param id 目标唯一 ID。
	 * @param type 目标语义类型。
	 * @param odometry 目标里程计状态。
	 */
	StaticObstacle(const Timestamp& timestamp,
				   UUID id,
				   ObstacleType type,
				   const Odometry& odometry)
		: Base(timestamp, id, type, ObstacleMotionType::STATIC_1, odometry)
	{
	}

	/** @brief 获取地图对象 ID。 */
	/** @return 地图对象 ID。 */
	UUID GetMapObjectID() const noexcept
	{
		return map_object_id_;
	}

	/** @brief 获取车道 ID。 */
	/** @return 车道 ID。 */
	UUID GetLaneID() const noexcept
	{
		return lane_id_;
	}

	/** @brief 获取道路边界 ID。 */
	/** @return 道路边界 ID。 */
	UUID GetRoadEdgeID() const noexcept
	{
		return road_edge_id_;
	}

	/** @brief 判断是否可通行。 */
	/** @return 可通行返回 true。 */
	bool IsPassable() const noexcept
	{
		return is_passable_;
	}

	/** @brief 判断是否临时障碍。 */
	/** @return 临时障碍返回 true。 */
	bool IsTemporary() const noexcept
	{
		return is_temporary_;
	}

	/** @brief 获取静态属性置信度。 */
	/** @return 静态属性置信度。 */
	T GetStaticConfidence() const noexcept
	{
		return static_confidence_;
	}

	/** @brief 获取连续观测时长。 */
	/** @return 连续观测时长（秒）。 */
	T GetLastObservedDurationSeconds() const noexcept
	{
		return last_observed_duration_s_;
	}

	/**
	 * @brief 设置地图对象 ID。
	 * @param map_object_id 地图对象 ID。
	 */
	void SetMapObjectID(UUID map_object_id) noexcept
	{
		map_object_id_ = map_object_id;
	}

	/**
	 * @brief 设置车道 ID。
	 * @param lane_id 车道 ID。
	 */
	void SetLaneID(UUID lane_id) noexcept
	{
		lane_id_ = lane_id;
	}

	/**
	 * @brief 设置道路边界 ID。
	 * @param road_edge_id 道路边界 ID。
	 */
	void SetRoadEdgeID(UUID road_edge_id) noexcept
	{
		road_edge_id_ = road_edge_id;
	}

	/**
	 * @brief 设置是否可通行。
	 * @param passable 可通行标记。
	 */
	void SetPassable(bool passable) noexcept
	{
		is_passable_ = passable;
	}

	/**
	 * @brief 设置是否临时障碍。
	 * @param temporary_flag 临时障碍标记。
	 */
	void SetTemporary(bool temporary_flag) noexcept
	{
		is_temporary_ = temporary_flag;
	}

	/**
	 * @brief 设置静态属性置信度。
	 * @param confidence 输入置信度，会被裁剪到 [0, 1]。
	 */
	void SetStaticConfidence(T confidence) noexcept
	{
		static_confidence_ = ClampUnit(confidence);
	}

	/**
	 * @brief 设置连续观测时长。
	 * @param duration_seconds 观测时长（秒），负值会被裁剪为 0。
	 */
	void SetLastObservedDurationSeconds(T duration_seconds) noexcept
	{
		last_observed_duration_s_ = ClampNonNegative(duration_seconds);
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