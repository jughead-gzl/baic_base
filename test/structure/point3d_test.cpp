#include <cassert>
#include <cmath>
#include <compare>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "../structure/base/point3d.h"

namespace
{
using Point2 = structure::Point2D<double>;
using Point3 = structure::Point3D<double>;

void TestConstructAndAccessors()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point3 p_default;
    // Verify default constructor initializes all coordinates to zero.
    assert(std::abs(p_default.GetX() - 0.0) <= kEps);
    assert(std::abs(p_default.GetY() - 0.0) <= kEps);
    assert(std::abs(p_default.GetZ() - 0.0) <= kEps);

    Point3 p(1.0, -2.0, 3.5);
    // Verify parameterized constructor stores x/y/z values.
    assert(std::abs(p.GetX() - 1.0) <= kEps);
    assert(std::abs(p.GetY() - (-2.0)) <= kEps);
    assert(std::abs(p.GetZ() - 3.5) <= kEps);

    Point2 p2(7.0, 8.0);
    Point3 from_point2(p2, -9.0);
    // Verify constructor from Point2D and z value.
    assert(std::abs(from_point2.GetX() - 7.0) <= kEps);
    assert(std::abs(from_point2.GetY() - 8.0) <= kEps);
    assert(std::abs(from_point2.GetZ() - (-9.0)) <= kEps);

    p.GetX() = 2.0;
    p.GetY() = 4.0;
    p.GetZ() = 6.0;
    // Verify non-const getters return mutable references.
    assert(std::abs(p.GetX() - 2.0) <= kEps);
    assert(std::abs(p.GetY() - 4.0) <= kEps);
    assert(std::abs(p.GetZ() - 6.0) <= kEps);

    p.SetZ(-1.5);
    // Verify SetZ updates z coordinate.
    assert(std::abs(p.GetZ() - (-1.5)) <= kEps);

    // Verify operator[] reads x, y, z by index.
    assert(std::abs(p[0] - 2.0) <= kEps);
    assert(std::abs(p[1] - 4.0) <= kEps);
    assert(std::abs(p[2] - (-1.5)) <= kEps);

    p[0] = 10.0;
    p[1] = -20.0;
    p[2] = 30.0;
    // Verify operator[] writes x, y, z by index.
    assert(std::abs(p.GetX() - 10.0) <= kEps);
    assert(std::abs(p.GetY() - (-20.0)) <= kEps);
    assert(std::abs(p.GetZ() - 30.0) <= kEps);

    static_assert(std::is_copy_constructible_v<Point3>);
    static_assert(std::is_copy_assignable_v<Point3>);
    static_assert(std::is_move_constructible_v<Point3>);
    static_assert(std::is_move_assignable_v<Point3>);

    Point3 copy_constructed(p);
    // Verify copy construction preserves all coordinates.
    assert(copy_constructed == p);

    Point3 copy_assigned;
    copy_assigned = p;
    // Verify copy assignment preserves all coordinates.
    assert(copy_assigned == p);

    Point3 moved_source(5.0, 6.0, 7.0);
    Point3 move_constructed(std::move(moved_source));
    // Verify move construction transfers all coordinates.
    assert(move_constructed == Point3(5.0, 6.0, 7.0));

    Point3 move_assigned;
    Point3 move_assign_source(-3.0, -4.0, -5.0);
    move_assigned = std::move(move_assign_source);
    // Verify move assignment transfers all coordinates.
    assert(move_assigned == Point3(-3.0, -4.0, -5.0));
}

void TestDistanceAPIs()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point3 a(1.0, 2.0, 3.0);
    Point3 b(4.0, -2.0, 7.0);

    // Verify Chebyshev distance with epsilon tolerance.
    assert(std::abs(a.DistOfChebyshev(b) - 4.0) <= kEps);
    // Verify Manhattan distance with epsilon tolerance.
    assert(std::abs(a.DistOfManhattan(b) - 11.0) <= kEps);
    // Verify Euclidean distance with epsilon tolerance.
    assert(std::abs(a.DistOfEuclidean(b) - std::sqrt(41.0)) <= kEps);
}

void TestTranslateAPIs()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point3 p(1.0, 2.0, 3.0);
    const Point3* addr = &p;

    const Point3& translated = p.Translate(3.0, -1.0, 5.0);
    // Verify Translate returns self reference.
    assert(&translated == addr);
    // Verify Translate updates x/y/z together.
    assert(std::abs(p.GetX() - 4.0) <= kEps);
    assert(std::abs(p.GetY() - 1.0) <= kEps);
    assert(std::abs(p.GetZ() - 8.0) <= kEps);

    const Point3& translated_z = p.TranslateZ(-2.0);
    // Verify TranslateZ returns self reference.
    assert(&translated_z == addr);
    // Verify TranslateZ only updates z.
    assert(std::abs(p.GetX() - 4.0) <= kEps);
    assert(std::abs(p.GetY() - 1.0) <= kEps);
    assert(std::abs(p.GetZ() - 6.0) <= kEps);

    const Point3& translated_xy = p.TranslateXY(1.0, 2.0);
    // Verify TranslateXY returns self reference.
    assert(&translated_xy == addr);
    // Verify TranslateXY updates x/y and keeps z unchanged.
    assert(std::abs(p.GetX() - 5.0) <= kEps);
    assert(std::abs(p.GetY() - 3.0) <= kEps);
    assert(std::abs(p.GetZ() - 6.0) <= kEps);

    const Point3& translated_xz = p.TranslateXZ(-2.0, 3.0);
    // Verify TranslateXZ returns self reference.
    assert(&translated_xz == addr);
    // Verify TranslateXZ updates x/z and keeps y unchanged.
    assert(std::abs(p.GetX() - 3.0) <= kEps);
    assert(std::abs(p.GetY() - 3.0) <= kEps);
    assert(std::abs(p.GetZ() - 9.0) <= kEps);

    const Point3& translated_yz = p.TranslateYZ(-1.0, -4.0);
    // Verify TranslateYZ returns self reference.
    assert(&translated_yz == addr);
    // Verify TranslateYZ updates y/z and keeps x unchanged.
    assert(std::abs(p.GetX() - 3.0) <= kEps);
    assert(std::abs(p.GetY() - 2.0) <= kEps);
    assert(std::abs(p.GetZ() - 5.0) <= kEps);
}

void TestScaleAndRotateAPIs()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point3 p(3.0, 4.0, 5.0);

    Point3 scaled_by_op = p * 2.0;
    // Verify operator* returns a new scaled 3D point.
    assert(std::abs(scaled_by_op.GetX() - 6.0) <= kEps);
    assert(std::abs(scaled_by_op.GetY() - 8.0) <= kEps);
    assert(std::abs(scaled_by_op.GetZ() - 10.0) <= kEps);

    const Point3* addr = &p;
    const Point3& scaled = p.Scale(2.0, -0.5, 3.0);
    // Verify Scale returns self reference.
    assert(&scaled == addr);
    // Verify Scale applies all three factors.
    assert(std::abs(p.GetX() - 6.0) <= kEps);
    assert(std::abs(p.GetY() - (-2.0)) <= kEps);
    assert(std::abs(p.GetZ() - 15.0) <= kEps);

    const Point3& scaled_z = p.ScaleZ(0.2);
    // Verify ScaleZ returns self reference.
    assert(&scaled_z == addr);
    // Verify ScaleZ only scales z.
    assert(std::abs(p.GetX() - 6.0) <= kEps);
    assert(std::abs(p.GetY() - (-2.0)) <= kEps);
    assert(std::abs(p.GetZ() - 3.0) <= kEps);

    const Point3& scaled_xy = p.ScaleXY(0.5, -1.0);
    // Verify ScaleXY returns self reference.
    assert(&scaled_xy == addr);
    // Verify ScaleXY scales x/y and keeps z unchanged.
    assert(std::abs(p.GetX() - 3.0) <= kEps);
    assert(std::abs(p.GetY() - 2.0) <= kEps);
    assert(std::abs(p.GetZ() - 3.0) <= kEps);

    const Point3& scaled_xz = p.ScaleXZ(2.0, 0.5);
    // Verify ScaleXZ returns self reference.
    assert(&scaled_xz == addr);
    // Verify ScaleXZ scales x/z and keeps y unchanged.
    assert(std::abs(p.GetX() - 6.0) <= kEps);
    assert(std::abs(p.GetY() - 2.0) <= kEps);
    assert(std::abs(p.GetZ() - 1.5) <= kEps);

    const Point3& scaled_yz = p.ScaleYZ(3.0, 2.0);
    // Verify ScaleYZ returns self reference.
    assert(&scaled_yz == addr);
    // Verify ScaleYZ scales y/z and keeps x unchanged.
    assert(std::abs(p.GetX() - 6.0) <= kEps);
    assert(std::abs(p.GetY() - 6.0) <= kEps);
    assert(std::abs(p.GetZ() - 3.0) <= kEps);

    Point3 r(1.0, 0.0, 9.0);
    const Point3& rotated = r.Rotate(90.0);
    // Verify Rotate returns self reference.
    assert(&rotated == &r);
    // Verify Rotate affects x/y and keeps z unchanged.
    assert(std::abs(r.GetX() - 0.0) <= kEps);
    assert(std::abs(r.GetY() - 1.0) <= kEps);
    assert(std::abs(r.GetZ() - 9.0) <= kEps);
}

void TestArithmeticAndComparison()
{
    Point3 a(1.0, 2.0, 3.0);
    Point3 b(3.0, 4.0, 5.0);

    Point3 sum = a + b;
    // Verify operator+ returns coordinate-wise sum.
    assert(sum == Point3(4.0, 6.0, 8.0));

    Point3 diff = b - a;
    // Verify operator- returns coordinate-wise difference.
    assert(diff == Point3(2.0, 2.0, 2.0));

    Point3 eq1(1.0, 1.0, 1.0);
    Point3 eq2(1.0, 1.0, 1.0);
    // Verify equality for same coordinates.
    assert(eq1 == eq2);
    // Verify inequality is negation of equality.
    assert(!(eq1 != eq2));

}

void TestVectorOpsAndUtilities()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point3 a(1.0, 2.0, 3.0);
    Point3 b(4.0, 5.0, 6.0);

    // Verify dot product value.
    assert(std::abs(a.Dot(b) - 32.0) <= kEps);

    Point3 cross = a.Cross(b);
    // Verify cross product coordinates.
    assert(std::abs(cross.GetX() - (-3.0)) <= kEps);
    assert(std::abs(cross.GetY() - 6.0) <= kEps);
    assert(std::abs(cross.GetZ() - (-3.0)) <= kEps);

    Point3 len_p(2.0, 3.0, 6.0);
    // Verify vector length calculation.
    assert(std::abs(len_p.Length() - 7.0) <= kEps);

    std::string s = len_p.ToString();
    // Verify string conversion format.
    assert(s == "(2.000000, 3.000000, 6.000000)");

    Point3 n(2.0, 3.0, 6.0);
    n.Normalize();
    // Verify normalize produces unit vector.
    assert(std::abs(n.Length() - 1.0) <= kEps);
    assert(std::abs(n.GetX() - (2.0 / 7.0)) <= kEps);
    assert(std::abs(n.GetY() - (3.0 / 7.0)) <= kEps);
    assert(std::abs(n.GetZ() - (6.0 / 7.0)) <= kEps);

    Point3 zero;
    zero.Normalize();
    // Verify zero vector remains unchanged after normalize.
    assert(std::abs(zero.GetX() - 0.0) <= kEps);
    assert(std::abs(zero.GetY() - 0.0) <= kEps);
    assert(std::abs(zero.GetZ() - 0.0) <= kEps);
}
} // namespace

int main()
{
    TestConstructAndAccessors();
    TestDistanceAPIs();
    TestTranslateAPIs();
    TestScaleAndRotateAPIs();
    TestArithmeticAndComparison();
    TestVectorOpsAndUtilities();
    return 0;
}
