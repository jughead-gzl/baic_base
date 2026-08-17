# PlanningPath 测试文档

## 1. 背景

`PlanningPath` 已调整为以 `Odometry<T>` 作为路径点类型，并提供以下核心接口：

- 类型别名：`PathPoint`、`PathPoints`
- 访问接口：`GetPathPoints()`、`GetFrameId()`、`GetTimestamp()`
- 修改接口：`SetPathPoints()`、`SetTimestamp()`、`SetFrameId()`
- 容器操作：`PushBack()`、`PushFront()`、`EmplaceBack()`、`EmplaceFront()`、`Insert()`、`PopBack()`、`PopFront()`
- 查询接口：`Empty()`、`Size()`、`Front()`、`Back()`、`At()`、`operator[]`
- 路径长度：`GetLength2D()`、`GetLength3D()`
- 文本输出：`ToString()`

测试文档同步以上接口变化，避免使用已过时命名（如 `Waypoint`、`GetWaypoints()`）。

## 2. 测试目标

1. 验证构造、访问、修改与容器操作行为正确。
2. 验证越界和空容器场景的异常行为符合预期。
3. 验证二维/三维路径长度计算正确。
4. 验证字符串输出包含关键字段。
5. 验证支持自定义底层容器（例如 `std::deque<Odometry<T>>`）。

## 3. 用例清单（与现有测试函数映射）

| 测试函数 | 目标 | 关键断言 |
| --- | --- | --- |
| `TestConstructAndAccessors` | 构造与访问器 | `frame_id`、`timestamp`、`Size`、路径点坐标读取正确 |
| `TestPushBackAndLength` | 追加与长度计算 | `EmplaceBack/PushBack` 后 `Size` 正确，`GetLength3D()` 为分段距离之和 |
| `TestCustomContainerType` | 自定义容器支持 | `PlanningPath<double, std::deque<...>>` 可构造并正常访问 |
| `TestFrontBackAndClear` | 首尾访问与清空 | 空路径访问抛异常；非空时 `Front/Back` 正确；`Clear` 后为空 |
| `TestToString` | 文本输出 | 输出包含 `PlanningPath{}`、`timestamp=`、`frame_id=`、`size=` |

## 4. 新接口对应验证点

为与当前实现保持一致，测试代码应满足以下命名与语义：

- 路径点类型使用 `Path::PathPoint`（等价于 `Odometry<double>`）。
- 路径容器访问使用 `GetPathPoints()`，不再使用 `GetWaypoints()`。
- 长度验证建议同时覆盖：
  - `GetLength2D()`：仅基于 XY 平面距离。
  - `GetLength3D()`：基于 XYZ 欧氏距离。

## 5. 异常与边界条件

建议至少覆盖以下边界行为：

1. 空路径调用 `Front()/Back()/PopFront()/PopBack()` 抛 `std::out_of_range`。
2. `At(index)` 和 `operator[](index)` 越界抛 `std::out_of_range`。
3. `Insert(index, point)` 在 `index > Size()` 时抛 `std::out_of_range`。
4. `GetLength2D()/GetLength3D()` 在点数小于 2 时返回 0。

## 6. 运行方式

在 `baic_base` 目录构建并执行：

```powershell
cmake -S . -B build
cmake --build build --target planning_path_test
./build/test/structure/planning_path_test
```

若平台输出路径不同，请以生成目录中的实际可执行文件位置为准。

## 7. 待补充项

当前建议继续补充以下测试：

1. `GetLength2D()` 与 `GetLength3D()` 的差异性场景（例如仅 Z 变化）。
2. `Insert(iterator, point)` 的中间位置插入行为。
3. `SetPathPoints()` 的拷贝/移动语义验证。
4. `ToString()` 中 `length3d=` 字段值与 `GetLength3D()` 一致性验证。
