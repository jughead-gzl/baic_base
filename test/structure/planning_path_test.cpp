#include <cassert>
#include <deque>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../structure/base/planning_path.h"

namespace
{
using Path = structure::PlanningPath<double>;
using PathPoint = Path::PathPoint;
using DequePath = structure::PlanningPath<double, std::deque<PathPoint>>;
using Pose = PathPoint::Pose;
using TS = structure::Timestamp;

void TestConstructAndAccessors()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    std::vector<PathPoint> points;
    points.emplace_back(TS::FromSeconds(0), Pose(0.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));
    points.emplace_back(TS::FromSeconds(1), Pose(1.0, 2.0, 3.0), Pose(), Pose(), std::string("map"), std::string("base_link"));

    Path path(TS::FromSeconds(5), points, std::string("map"));

    assert(path.GetFrameId() == "map");
    assert(path.GetTimestamp().Nanoseconds() == TS::FromSeconds(5).Nanoseconds());
    assert(path.Size() == 2);
    assert(std::abs(path[1].GetPose().GetX() - 1.0) <= kEps);
    assert(std::abs(path[1].GetPose().GetZ() - 3.0) <= kEps);
}

void TestPushBackAndLength()
{
    constexpr double kEps = 1e-12;

    Path path;
    path.EmplaceBack(TS::FromSeconds(0), Pose(0.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));
    path.EmplaceBack(TS::FromSeconds(1), Pose(3.0, 4.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));
    path.PushBack(PathPoint(TS::FromSeconds(2), Pose(3.0, 4.0, 12.0), Pose(), Pose(), std::string("map"), std::string("base_link")));

    // 2D length is |(0,0)->(3,4)| + |(3,4)->(3,4)| = 5 + 0.
    assert(std::abs(path.GetLength2D() - 5.0) <= kEps);
    // Length is |(0,0,0)->(3,4,0)| + |(3,4,0)->(3,4,12)| = 5 + 12.
    assert(std::abs(path.GetLength3D() - 17.0) <= kEps);
    assert(path.Size() == 3);
}

void TestCustomContainerType()
{
    std::deque<PathPoint> points;
    points.emplace_back(TS::FromSeconds(10), Pose(1.0, 2.0, 3.0), Pose(), Pose(), std::string("odom"), std::string("base_link"));
    points.emplace_back(TS::FromSeconds(11), Pose(4.0, 5.0, 6.0), Pose(), Pose(), std::string("odom"), std::string("base_link"));

    DequePath path(points, std::string("base_link"));

    assert(path.GetFrameId() == "base_link");
    assert(path.Size() == 2);
    assert(path.GetPathPoints().size() == 2);
    assert(path.GetPathPoints().front().GetPose().GetX() == 1.0);
}

void TestInsertAndOutOfRange()
{
    Path path;

    bool thrown = false;
    try
    {
        (void)path.At(0);
    }
    catch (const std::out_of_range&)
    {
        thrown = true;
    }
    assert(thrown);

    path.EmplaceBack(TS::FromSeconds(0), Pose(0.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));
    path.EmplaceBack(TS::FromSeconds(1), Pose(2.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));

    path.Insert(1, PathPoint(TS::FromSeconds(2), Pose(1.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link")));
    assert(path.Size() == 3);
    assert(path[1].GetPose().GetX() == 1.0);

    bool insert_thrown = false;
    try
    {
        path.Insert(10, PathPoint(TS::FromSeconds(3), Pose(3.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link")));
    }
    catch (const std::out_of_range&)
    {
        insert_thrown = true;
    }
    assert(insert_thrown);
}

void TestFrontBackAndClear()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Path path;

    bool thrown = false;
    try
    {
        (void)path.Front();
    }
    catch (const std::out_of_range&)
    {
        thrown = true;
    }
    assert(thrown);

    path.EmplaceBack(TS::FromSeconds(1), Pose(1.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));
    path.EmplaceBack(TS::FromSeconds(2), Pose(2.0, 0.0, 0.0), Pose(), Pose(), std::string("map"), std::string("base_link"));

    assert(std::abs(path.Front().GetPose().GetX() - 1.0) <= kEps);
    assert(std::abs(path.Back().GetPose().GetX() - 2.0) <= kEps);

    path.Clear();
    assert(path.Empty());
}

void TestToString()
{
    Path path;
    path.SetTimestamp(TS::FromSeconds(7));
    path.SetFrameId(std::string("odom"));
    path.EmplaceBack(TS::FromSeconds(0), Pose(0.0, 0.0, 0.0), Pose(), Pose(), std::string("odom"), std::string("base_link"));

    const std::string text = path.ToString();
    assert(!text.empty());
    assert(text.find("PlanningPath{") != std::string::npos);
    assert(text.find("timestamp=") != std::string::npos);
    assert(text.find("frame_id=odom") != std::string::npos);
    assert(text.find("size=1") != std::string::npos);
}
} // namespace

int main()
{
    TestConstructAndAccessors();
    TestPushBackAndLength();
    TestCustomContainerType();
    TestInsertAndOutOfRange();
    TestFrontBackAndClear();
    TestToString();
    return 0;
}
