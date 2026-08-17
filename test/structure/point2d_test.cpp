#include <cassert>
#include <cmath>
#include <compare>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "../structure/base/point2d.h"

namespace
{
using Point = structure::Point2D<double>;

void TestConstructAndAccessors()
{
    Point p_default;
    // Verify default constructor initializes to origin.
    assert(p_default == Point(0.0, 0.0));

    Point p(1.5, -2.5);
    // Verify parameterized constructor stores x/y values.
    assert(p == Point(1.5, -2.5));

    p.GetX() = 3.0;
    p.GetY() = 4.0;
    // Verify non-const getters return mutable references.
    assert(p == Point(3.0, 4.0));

    p.SetX(-1.25);
    p.SetY(2.75);
    // Verify setters update both coordinates correctly.
    assert(p == Point(-1.25, 2.75));

    const Point cp(8.0, 9.0);
    // Verify const getters expose constructor values.
    assert(cp == Point(8.0, 9.0));

    static_assert(std::is_copy_constructible_v<Point>);
    static_assert(std::is_copy_assignable_v<Point>);
    static_assert(std::is_move_constructible_v<Point>);
    static_assert(std::is_move_assignable_v<Point>);

    Point copy_constructed(cp);
    // Verify copy construction preserves coordinate values.
    assert(copy_constructed == cp);

    Point copy_assigned;
    copy_assigned = cp;
    // Verify copy assignment preserves coordinate values.
    assert(copy_assigned == cp);

    Point moved_source(5.0, 6.0);
    Point move_constructed(std::move(moved_source));
    // Verify move construction transfers coordinate values.
    assert(move_constructed == Point(5.0, 6.0));

    Point move_assigned;
    Point move_assign_source(7.0, 8.0);
    move_assigned = std::move(move_assign_source);
    // Verify move assignment transfers coordinate values.
    assert(move_assigned == Point(7.0, 8.0));
}

void TestDistanceAPIs()
{
    Point a(1.0, 2.0);
    Point b(4.0, -2.0);
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    // Verify Chebyshev distance with epsilon tolerance.
    assert(std::abs(a.DistOfChebyshev(b) - 4.0) <= kEps);
    // Verify Manhattan distance with epsilon tolerance.
    assert(std::abs(a.DistOfManhattan(b) - 7.0) <= kEps);
    // Verify Euclidean distance with epsilon tolerance.
    assert(std::abs(a.DistOfEuclidean(b) - 5.0) <= kEps);
}

void TestIndexOperatorAPIs()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();
    Point p(1.0, 2.0);

    // Verify index 0 reads x coordinate.
    assert(std::abs(p[0] - 1.0) <= kEps);
    // Verify index 1 reads y coordinate.
    assert(std::abs(p[1] - 2.0) <= kEps);

    p[0] = 3.5;
    p[1] = -4.5;
    // Verify index 0/1 can modify coordinates.
    assert(std::abs(p.GetX() - 3.5) <= kEps);
    assert(std::abs(p.GetY() - (-4.5)) <= kEps);
}

void TestTranslateAPIs()
{
    Point p(1.0, 2.0);
    Point delta(1.5, -0.5);

    const Point* addr = &p;
    const Point& translated = p.Translate(3.0, -1.0);
    // Verify Translate returns self reference.
    assert(&translated == addr);
    // Verify Translate applies both offsets.
    assert(p == Point(4.0, 1.0));

    const Point& translated_x = p.TranslateX(2.5);
    // Verify TranslateX returns self reference.
    assert(&translated_x == addr);
    // Verify TranslateX only updates x.
    assert(p == Point(6.5, 1.0));

    const Point& translated_y = p.TranslateY(-3.0);
    // Verify TranslateY returns self reference.
    assert(&translated_y == addr);
    // Verify TranslateY only updates y.
    assert(p == Point(6.5, -2.0));

    Point sum = p + delta;
    // Verify operator+ returns coordinate-wise sum.
    assert(sum == Point(8.0, -2.5));

    Point diff = p - delta;
    // Verify operator- returns coordinate-wise difference.
    assert(diff == Point(5.0, -1.5));
}

void TestScaleAPIs()
{
    Point p(6.5, -2.0);

    Point scaled_by_op = p * 2.0;
    // Verify operator* returns a new scaled point.
    assert(scaled_by_op == Point(13.0, -4.0));

    const Point* addr = &p;
    const Point& scaled = p.Scale(2.0, -0.5);
    // Verify Scale returns self reference.
    assert(&scaled == addr);
    // Verify Scale applies x/y factors.
    assert(p == Point(13.0, 1.0));

    const Point& scaled_x = p.ScaleX(0.5);
    // Verify ScaleX returns self reference.
    assert(&scaled_x == addr);
    // Verify ScaleX only scales x.
    assert(p == Point(6.5, 1.0));

    const Point& scaled_y = p.ScaleY(-2.0);
    // Verify ScaleY returns self reference.
    assert(&scaled_y == addr);
    // Verify ScaleY only scales y.
    assert(p == Point(6.5, -2.0));
}

void TestRotationAPIs()
{
    Point p1(1.0, 0.0);
    p1.Rotate(90.0);
    // Verify Rotate puts point at expected location.
    assert(Point(p1.DistOfEuclidean(Point(0.0, 1.0)), 0.0) == Point(0.0, 0.0));

    Point p2(1.0, 0.0);
    Point rotated_cw = p2 > 90.0;
    // Verify operator> rotation result.
    assert(Point(rotated_cw.DistOfEuclidean(Point(0.0, 1.0)), 0.0) == Point(0.0, 0.0));

    Point rotated_ccw = p2 < 90.0;
    // Verify operator< rotation result.
    assert(Point(rotated_ccw.DistOfEuclidean(Point(0.0, -1.0)), 0.0) == Point(0.0, 0.0));
}

void TestArithmeticAndComparison()
{
    Point a(1.0, 2.0);
    Point b(3.0, 4.0);

    Point scaled = a * 2.0;
    // Verify scalar multiplication operator.
    assert(scaled == Point(2.0, 4.0));

    Point sum = a + b;
    // Verify addition operator.
    assert(sum == Point(4.0, 6.0));

    Point diff = b - a;
    // Verify subtraction operator.
    assert(diff == Point(2.0, 2.0));

    Point eq1(1.0, 1.0);
    Point eq2(1.0 + std::numeric_limits<double>::epsilon() * 0.5, 1.0);
    // Verify epsilon-based equality for floating points.
    assert(eq1 == eq2);
    // Verify inequality is logical negation of equality.
    assert(!(eq1 != eq2));

}

void TestVectorOpsAndUtilities()
{
    Point a(3.0, 4.0);
    Point b(1.0, 2.0);
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    // Verify dot product result.
    assert(std::abs(a.Dot(b) - 11.0) <= kEps);

    Point cross = a.Cross(b);
    // Verify 2D cross product representation.
    assert(std::abs(cross.GetX() - 2.0) <= kEps);
    assert(std::abs(cross.GetY() - 0.0) <= kEps);

    // Verify vector length computation.
    assert(std::abs(a.Length() - 5.0) <= kEps);

    std::string s = a.ToString();
    // Verify string formatting output.
    assert(s == "(3.000000, 4.000000)");

    Point n(3.0, 4.0);
    n.Normalize();
    // Verify normalization produces unit length.
    assert(std::abs(n.Length() - 1.0) <= kEps);
    assert(std::abs(n.GetX() - 0.6) <= kEps);
    assert(std::abs(n.GetY() - 0.8) <= kEps);

    Point zero;
    zero.Normalize();
    // Verify zero vector stays unchanged after normalize.
    assert(std::abs(zero.GetX() - 0.0) <= kEps);
    assert(std::abs(zero.GetY() - 0.0) <= kEps);
}
} // namespace

int main()
{
    TestConstructAndAccessors();
    TestDistanceAPIs();
    TestIndexOperatorAPIs();
    TestTranslateAPIs();
    TestScaleAPIs();
    TestRotationAPIs();
    TestArithmeticAndComparison();
    TestVectorOpsAndUtilities();
    return 0;
}
