#pragma once

#include "..\common.h"

#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

namespace structure
{
namespace perception
{
/**
 * @brief 车位几何形态。
 */
enum class ParkingSlotGeometryType : uint8_t
{
    UNKNOWN_0,        /**< 未知形态。 */
    PERPENDICULAR_1,  /**< 垂直车位。 */
    PARALLEL_2,       /**< 平行车位。 */
    DIAGONAL_3        /**< 斜列车位。 */
};

/**
 * @brief 车位物理类型。
 */
enum class ParkingSlotPhysicalType : uint8_t
{
    UNKNOWN_0,     /**< 未知类型。 */
    COMMON_1,      /**< 普通地面车位。 */
    SPACE_2,       /**< 空旷可停区域抽象车位。 */
    MECHANICAL_3   /**< 机械车位。 */
};

/**
 * @brief 停车位基础描述类。
 *
 * 使用中心点和 4 个顶点描述车位几何，并附带基础识别属性。
 */
template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<std::decay_t<T>>>>
class ParkingSlot
{
public:
    /** @brief 顶点元素类型。 */
    using Vertex = Vertex3D<T>;
    /** @brief 顶点数组类型（顺时针或逆时针存储四边形顶点）。 */
    using Vertices = std::array<Vertex, 4>;
    /** @brief 兼容旧拼写：车位顶点数组类型。 */
    using Vertics = Vertices;
    /** @brief 中心点类型。 */
    using Center = Postion3D<T>;
    /** @brief 车位入口的两个顶点类型。 */
    using EntranceVertices = std::array<Vertex, 2>;
    /** @brief 车位入口顶点下标类型。 */
    using EntranceVertexIndices = std::array<std::size_t, 2>;
    /** @brief 识别置信度。 */
    using Confidence = T;
    /** @brief 最大 size_t 值。 */
    static constexpr std::size_t MaxSizeT = std::numeric_limits<std::size_t>::max();

private:
    /** @brief 数据时间戳。 */
    Timestamp timestamp_{};
    /** @brief 车位唯一 ID。 */
    UUID id_{0};
    /** @brief 车位几何形态。 */
    ParkingSlotGeometryType geometry_type_{ParkingSlotGeometryType::UNKNOWN_0};
    /** @brief 车位物理类型。 */
    ParkingSlotPhysicalType physical_type_{ParkingSlotPhysicalType::UNKNOWN_0};
    /** @brief 车位中心点。 */
    Center center_{};
    /** @brief 车位四个顶点。 */
    Vertices vertices_{};
    /** @brief 车位入口的两个顶点。 */
    EntranceVertices entrance_vertices_{};
    /** @brief 车位入口两个顶点在车位顶点数组中的下标。 */
    EntranceVertexIndices entrance_indices_{MaxSizeT, MaxSizeT};
    /** @brief 当前车位是否可用。 */
    bool is_available_{false};
    /** @brief 识别置信度，范围建议为 [0, 1]。 */
    Confidence confidence_{static_cast<T>(0)};

public:
    /** @brief 默认构造函数。 */
    ParkingSlot() = default;

    /**
     * @brief 构造停车位对象。
     * @param timestamp 数据时间戳。
     * @param id 车位 ID。
     * @param geometry_type 几何形态。
     * @param physical_type 物理类型。
     * @param center 中心点。
     * @param vertices 4 个顶点。
     */
    ParkingSlot(const Timestamp& timestamp,
                UUID id,
                ParkingSlotGeometryType geometry_type,
                ParkingSlotPhysicalType physical_type,
                const Center& center,
                const Vertices& vertices)
        : timestamp_(timestamp),
          id_(id),
          geometry_type_(geometry_type),
          physical_type_(physical_type),
          center_(center),
          vertices_(vertices)
    {
    }

public:
    /** @brief 获取数据时间戳。 */
    /** @return 时间戳只读引用。 */
    const Timestamp& GetTimestamp() const noexcept
    {
        return timestamp_;
    }

    /** @brief 获取车位唯一 ID。 */
    /** @return 车位 ID。 */
    UUID GetID() const noexcept
    {
        return id_;
    }

    /** @brief 获取几何形态。 */
    /** @return 车位几何形态枚举值。 */
    ParkingSlotGeometryType GetGeometryType() const noexcept
    {
        return geometry_type_;
    }

    /** @brief 获取物理类型。 */
    /** @return 车位物理类型枚举值。 */
    ParkingSlotPhysicalType GetPhysicalType() const noexcept
    {
        return physical_type_;
    }

    /** @brief 获取中心点（只读）。 */
    /** @return 中心点只读引用。 */
    const Center& GetCenter() const noexcept
    {
        return center_;
    }

    /** @brief 获取中心点（可写）。 */
    /** @return 中心点可写引用。 */
    Center& GetCenter() noexcept
    {
        return center_;
    }

    /** @brief 获取车位入口的两个顶点（只读）。 */
    /** @return 入口顶点只读数组。 */
    const EntranceVertices& GetEntranceVertices() const noexcept
    {
        return entrance_vertices_;
    }

    /** @brief 获取车位入口的两个顶点（可写）。 */
    /** @return 入口顶点可写数组。 */
    EntranceVertices& GetEntranceVertices() noexcept
    {
        return entrance_vertices_;
    }

    /** @brief 获取入口顶点在车位顶点数组中的下标（只读）。 */
    /** @return 入口顶点下标只读数组。 */
    const EntranceVertexIndices& GetEntranceVertexIndices() const noexcept
    {
        return entrance_indices_;
    }

    /** @brief 获取入口顶点在车位顶点数组中的下标（可写）。 */
    /** @return 入口顶点下标可写数组。 */
    EntranceVertexIndices& GetEntranceVertexIndices() noexcept
    {
        return entrance_indices_;
    }

    /**
     * @brief 获取指定索引的顶点（只读）。
     * @param index 顶点索引，范围为 [0, 3]
     * @return 指定索引的顶点只读引用。
     * @throws std::out_of_range 如果索引超出范围。
     */
    const Vertex& At(std::size_t index) const
    {
        if (index >= vertices_.size())
        {
            throw std::out_of_range("Index out of range for ParkingSlot vertices");
        }
        return vertices_[index];
    }

    /**
     * @brief 获取指定索引的顶点（可写）。
     * @param index 顶点索引，范围为 [0, 3]
     * @return 指定索引的顶点可写引用。
     * @throws std::out_of_range 如果索引超出范围。
     */
    Vertex& operator[](std::size_t index)
    {
        if (index >= vertices_.size())
        {
            throw std::out_of_range("Index out of range for ParkingSlot vertices");
        }
        return vertices_[index];
    }

    /** @brief 获取四个顶点（只读）。 */
    /** @return 顶点数组只读引用。 */
    const Vertices& GetVertices() const noexcept
    {
        return vertices_;
    }

    /** @brief 获取四个顶点（可写）。 */
    /** @return 顶点数组可写引用。 */
    Vertices& GetVertices() noexcept
    {
        return vertices_;
    }

    /** @brief 查询车位可用状态。 */
    /** @return 可用返回 true。 */
    bool IsAvailable() const noexcept
    {
        return is_available_;
    }

    /** @brief 获取识别置信度。 */
    /** @return 置信度，范围通常为 [0, 1]。 */
    Confidence GetConfidence() const noexcept
    {
        return confidence_;
    }

    /** @brief 判断几何是否有效。 */
    /** @return 面积大于 0 返回 true。 */
    bool HasValidGeometry() const noexcept
    {
        return GetArea2D() > static_cast<T>(0);
    }

    /**
     * @brief 计算车位平面面积。
     * @return 基于四边形顶点的 2D 面积（XY 平面）。
     */
    T GetArea2D() const noexcept
    {
        T twice_area = static_cast<T>(0);
        for (std::size_t i = 0; i < vertices_.size(); ++i)
        {
            const auto& p = vertices_[i];
            const auto& q = vertices_[(i + 1) % vertices_.size()];
            twice_area += (p.GetX() * q.GetY() - q.GetX() * p.GetY());
        }
        return static_cast<T>(0.5) * static_cast<T>(std::abs(static_cast<double>(twice_area)));
    }

    /**
     * @brief 计算车位长边长度（XY 平面）。
     */
    T GetLength2D() const noexcept
    {
        const T e0 = vertices_[0].Dist2DOfEuclidean(vertices_[1]);
        const T e1 = vertices_[1].Dist2DOfEuclidean(vertices_[2]);
        const T e2 = vertices_[2].Dist2DOfEuclidean(vertices_[3]);
        const T e3 = vertices_[3].Dist2DOfEuclidean(vertices_[0]);
        return std::max(std::max(e0, e1), std::max(e2, e3));
    }

    /**
     * @brief 计算车位短边长度（XY 平面）。
     */
    T GetWidth2D() const noexcept
    {
        const T e0 = vertices_[0].Dist2DOfEuclidean(vertices_[1]);
        const T e1 = vertices_[1].Dist2DOfEuclidean(vertices_[2]);
        const T e2 = vertices_[2].Dist2DOfEuclidean(vertices_[3]);
        const T e3 = vertices_[3].Dist2DOfEuclidean(vertices_[0]);
        return std::min(std::min(e0, e1), std::min(e2, e3));
    }

public:
    /**
     * @brief 设置数据时间戳。
     * @tparam U 时间戳类型，需与 Timestamp 等价。
     * @param timestamp 新时间戳。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Timestamp>>>
    void SetTimestamp(U&& timestamp) noexcept
    {
        timestamp_ = std::forward<U>(timestamp);
    }

    /**
     * @brief 设置车位 ID。
     * @param id 新车位 ID。
     */
    void SetID(UUID id = 0) noexcept
    {
        id_ = id;
    }

    /**
     * @brief 设置几何形态。
     * @param geometry_type 新几何形态。
     */
    void SetGeometryType(ParkingSlotGeometryType geometry_type = ParkingSlotGeometryType::UNKNOWN_0) noexcept
    {
        geometry_type_ = geometry_type;
    }

    /**
     * @brief 设置物理类型。
     * @param physical_type 新物理类型。
     */
    void SetPhysicalType(ParkingSlotPhysicalType physical_type = ParkingSlotPhysicalType::UNKNOWN_0) noexcept
    {
        physical_type_ = physical_type;
    }

    /**
     * @brief 设置中心点。
     * @tparam U 中心点类型，需与 Center 等价。
     * @param center 新中心点。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Center>>>
    void SetCenter(U&& center) noexcept
    {
        center_ = std::forward<U>(center);
    }

    /**
     * @brief 设置车位入口的两个顶点。
     * @tparam U 入口顶点数组类型，需与 EntranceVertices 等价。
     * @param entrance_vertices 新车位入口顶点。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, EntranceVertices>>>
    void SetEntranceVertices(U&& entrance_vertices) noexcept
    {
        entrance_vertices_ = std::forward<U>(entrance_vertices);
    }

    /**
     * @brief 设置入口顶点在车位顶点数组中的下标。
     * @tparam U 入口顶点下标数组类型，需与 EntranceVertexIndices 等价。
     * @param entrance_vertex_indices 新入口顶点下标。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, EntranceVertexIndices>>>
    void SetEntranceVertexIndices(U&& entrance_vertex_indices) noexcept
    {
        entrance_indices_ = std::forward<U>(entrance_vertex_indices);
    }

    /**
     * @brief 设置四个顶点。
     * @tparam U 顶点数组类型，需与 Vertices 等价。
     * @param vertices 新顶点数组。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Vertices>>>
    void SetVertices(U&& vertices) noexcept
    {
        vertices_ = std::forward<U>(vertices);
    }

    /**
     * @brief 设置指定索引的顶点。
     * @tparam U 顶点类型，需与 Vertex 等价。
     * @param index 顶点索引，范围为 [0, 3]
     * @param vertex 新顶点。
     * @throws std::out_of_range 如果索引超出范围。
     */
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Vertex>>>
    void SetVertex(std::size_t index, U && vertex)
    {
        if (index >= vertices_.size())
        {
            throw std::out_of_range("Index out of range for ParkingSlot vertices");
        }
        vertices_[index] = std::forward<U>(vertex);
    }

    /**
     * @brief 设置可用状态。
     * @param available 可用标记。
     */
    void SetAvailable(bool available) noexcept
    {
        is_available_ = available;
    }

    /**
     * @brief 设置置信度。
     * @param confidence 输入置信度，会被裁剪到 [0, 1]。
     */
    void SetConfidence(Confidence confidence) noexcept
    {
        confidence_ = ClampUnit(confidence);
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
};
};
};

