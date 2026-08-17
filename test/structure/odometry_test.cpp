#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include "../../structure/algorithm/alam/odometry.h"

namespace
{
using Odom = structure::Odometry<double>;
using Pose = Odom::Pose;
using TS = structure::Timestamp;

void AssertPoseNear(const Pose& pose,
                    double x,
                    double y,
                    double z,
                    double roll,
                    double pitch,
                    double yaw,
                    double eps = std::numeric_limits<double>::epsilon())
{
    assert(std::abs(pose.GetX() - x) <= eps);
    assert(std::abs(pose.GetY() - y) <= eps);
    assert(std::abs(pose.GetZ() - z) <= eps);
    assert(std::abs(pose.GetRoll() - roll) <= eps);
    assert(std::abs(pose.GetPitch() - pitch) <= eps);
    assert(std::abs(pose.GetYaw() - yaw) <= eps);
}

void TestDefaultConstructor()
{
    Odom odom;

    assert(odom.GetTimestamp().Nanoseconds() == 0);
    AssertPoseNear(odom.GetPose(), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    AssertPoseNear(odom.GetTwist(), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    AssertPoseNear(odom.GetAccel(), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    assert(odom.GetFrameId() == "map");
    assert(odom.GetChildFrameId() == "base_link");
}

void TestParameterizedConstructorAndAccessors()
{
    TS timestamp = TS::FromSeconds(42);
    Pose pose(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    Pose twist(4.0, 5.0, 6.0, 0.4, 0.5, 0.6);
    Pose accel(7.0, 8.0, 9.0, 0.7, 0.8, 0.9);
    std::string frame_id = "odom";
    std::string child_frame_id = "ego";

    Odom odom(std::move(timestamp), std::move(pose), std::move(twist), std::move(accel), std::move(frame_id), std::move(child_frame_id));

    assert(odom.GetTimestamp().Nanoseconds() == TS::FromSeconds(42).Nanoseconds());
    AssertPoseNear(odom.GetPose(), 1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    AssertPoseNear(odom.GetTwist(), 4.0, 5.0, 6.0, 0.4, 0.5, 0.6);
    AssertPoseNear(odom.GetAccel(), 7.0, 8.0, 9.0, 0.7, 0.8, 0.9);
    assert(odom.GetFrameId() == "odom");
    assert(odom.GetChildFrameId() == "ego");
}

void TestSettersAndMutableGetters()
{
    Odom odom;

    odom.SetTimestamp(TS::FromMilliseconds(123));
    odom.SetPose(Pose(1.0, 1.0, 1.0, 0.1, 0.1, 0.1));
    odom.SetTwist(Pose(2.0, 2.0, 2.0, 0.2, 0.2, 0.2));
    odom.SetAccel(Pose(3.0, 3.0, 3.0, 0.3, 0.3, 0.3));
    odom.SetFrameId(std::string("map_local"));
    odom.SetChildFrameId(std::string("base"));

    assert(odom.GetTimestamp().Nanoseconds() == TS::FromMilliseconds(123).Nanoseconds());
    AssertPoseNear(odom.GetPose(), 1.0, 1.0, 1.0, 0.1, 0.1, 0.1);
    AssertPoseNear(odom.GetTwist(), 2.0, 2.0, 2.0, 0.2, 0.2, 0.2);
    AssertPoseNear(odom.GetAccel(), 3.0, 3.0, 3.0, 0.3, 0.3, 0.3);
    assert(odom.GetFrameId() == "map_local");
    assert(odom.GetChildFrameId() == "base");

    odom.GetPose().TranslatePose(1.0, -1.0, 2.0, 0.01, 0.02, 0.03);
    AssertPoseNear(odom.GetPose(), 2.0, 0.0, 3.0, 0.11, 0.12, 0.13);
}

void TestPredictConstantAcceleration()
{
    TS timestamp = TS::FromSeconds(1);
    Pose pose(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    Pose twist(10.0, 20.0, 30.0, 1.0, 2.0, 3.0);
    Pose accel(2.0, 4.0, 6.0, 0.2, 0.4, 0.6);
    std::string frame_id = "map";
    std::string child_frame_id = "base_link";

    Odom odom(timestamp, pose, twist, accel, frame_id, child_frame_id);

    odom.Predict(2.0);

    AssertPoseNear(odom.GetPose(), 25.0, 50.0, 75.0, 2.5, 5.0, 7.5);
    AssertPoseNear(odom.GetTwist(), 14.0, 28.0, 42.0, 1.4, 2.8, 4.2);
    AssertPoseNear(odom.GetAccel(), 2.0, 4.0, 6.0, 0.2, 0.4, 0.6);
    assert(odom.GetTimestamp().Nanoseconds() == TS::FromSeconds(3).Nanoseconds());
}

void TestPredictZeroDt()
{
    TS timestamp = TS::FromMilliseconds(10);
    Pose pose(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    Pose twist(4.0, 5.0, 6.0, 0.4, 0.5, 0.6);
    Pose accel(7.0, 8.0, 9.0, 0.7, 0.8, 0.9);
    std::string frame_id = "odom";
    std::string child_frame_id = "ego";

    Odom odom(timestamp, pose, twist, accel, frame_id, child_frame_id);

    odom.Predict(0.0);

    AssertPoseNear(odom.GetPose(), 1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    AssertPoseNear(odom.GetTwist(), 4.0, 5.0, 6.0, 0.4, 0.5, 0.6);
    AssertPoseNear(odom.GetAccel(), 7.0, 8.0, 9.0, 0.7, 0.8, 0.9);
    assert(odom.GetTimestamp().Nanoseconds() == TS::FromMilliseconds(10).Nanoseconds());
}

void TestPredictInvalidDtThrows()
{
    Odom odom;

    bool neg_thrown = false;
    try
    {
        odom.Predict(-1.0);
    }
    catch (const std::invalid_argument&)
    {
        neg_thrown = true;
    }
    assert(neg_thrown);

    bool inf_thrown = false;
    try
    {
        odom.Predict(std::numeric_limits<double>::infinity());
    }
    catch (const std::invalid_argument&)
    {
        inf_thrown = true;
    }
    assert(inf_thrown);

    bool nan_thrown = false;
    try
    {
        odom.Predict(std::numeric_limits<double>::quiet_NaN());
    }
    catch (const std::invalid_argument&)
    {
        nan_thrown = true;
    }
    assert(nan_thrown);
}

void TestToString()
{
    TS timestamp = TS::FromSeconds(7);
    Pose pose(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    Pose twist(4.0, 5.0, 6.0, 0.4, 0.5, 0.6);
    Pose accel(7.0, 8.0, 9.0, 0.7, 0.8, 0.9);
    std::string frame_id = "odom";
    std::string child_frame_id = "base_link";

    Odom odom(timestamp, pose, twist, accel, frame_id, child_frame_id);

    const std::string text = odom.ToString();
    assert(!text.empty());
    assert(text.find("Odometry{t=") != std::string::npos);
    assert(text.find("frame_id=odom") != std::string::npos);
    assert(text.find("child_frame_id=base_link") != std::string::npos);
    assert(text.find("pose=") != std::string::npos);
    assert(text.find("twist=") != std::string::npos);
    assert(text.find("accel=") != std::string::npos);
}
} // namespace

int main()
{
    TestDefaultConstructor();
    TestParameterizedConstructorAndAccessors();
    TestSettersAndMutableGetters();
    TestPredictConstantAcceleration();
    TestPredictZeroDt();
    TestPredictInvalidDtThrows();
    TestToString();
    return 0;
}
