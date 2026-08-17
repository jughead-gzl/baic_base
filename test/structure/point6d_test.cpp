#include <cassert>
#include <cmath>
#include <limits>
#include <string>

#include "../structure/base/point6d.h"

namespace
{
using Point6 = structure::Point6D<double>;

void TestConstructAndAccessors()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point6 p(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);

    // Verify position and orientation getters.
    assert(std::abs(p.GetX() - 1.0) <= kEps);
    assert(std::abs(p.GetY() - 2.0) <= kEps);
    assert(std::abs(p.GetZ() - 3.0) <= kEps);
    assert(std::abs(p.GetRoll() - 0.1) <= kEps);
    assert(std::abs(p.GetPitch() - 0.2) <= kEps);
    assert(std::abs(p.GetYaw() - 0.3) <= kEps);

    p.SetRoll(1.1);
    p.SetPitch(1.2);
    p.SetYaw(1.3);
    p.SetRPY(2.1, 2.2, 2.3);

    // Verify orientation setters.
    assert(std::abs(p.GetRoll() - 2.1) <= kEps);
    assert(std::abs(p.GetPitch() - 2.2) <= kEps);
    assert(std::abs(p.GetYaw() - 2.3) <= kEps);
}

void TestIndexOperator()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point6 p(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    // Verify index read mapping for all 6 DOF.
    assert(std::abs(p[0] - 1.0) <= kEps);
    assert(std::abs(p[1] - 2.0) <= kEps);
    assert(std::abs(p[2] - 3.0) <= kEps);
    assert(std::abs(p[3] - 4.0) <= kEps);
    assert(std::abs(p[4] - 5.0) <= kEps);
    assert(std::abs(p[5] - 6.0) <= kEps);

    p[0] = -1.0;
    p[1] = -2.0;
    p[2] = -3.0;
    p[3] = -4.0;
    p[4] = -5.0;
    p[5] = -6.0;

    // Verify index write mapping for all 6 DOF.
    assert(std::abs(p.GetX() - (-1.0)) <= kEps);
    assert(std::abs(p.GetY() - (-2.0)) <= kEps);
    assert(std::abs(p.GetZ() - (-3.0)) <= kEps);
    assert(std::abs(p.GetRoll() - (-4.0)) <= kEps);
    assert(std::abs(p.GetPitch() - (-5.0)) <= kEps);
    assert(std::abs(p.GetYaw() - (-6.0)) <= kEps);
}

void TestTranslateAndArithmetic()
{
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    Point6 p(1.0, 2.0, 3.0, 0.1, 0.2, 0.3);
    p.Translate(1.0, -1.0, 2.0);
    p.TranslateRPY(0.1, 0.2, 0.3);
    p.TranslatePose(1.0, 1.0, 1.0, 0.1, 0.1, 0.1);

    // Verify pose translation APIs.
    assert(std::abs(p.GetX() - 3.0) <= kEps);
    assert(std::abs(p.GetY() - 2.0) <= kEps);
    assert(std::abs(p.GetZ() - 6.0) <= kEps);
    assert(std::abs(p.GetRoll() - 0.3) <= kEps);
    assert(std::abs(p.GetPitch() - 0.5) <= kEps);
    assert(std::abs(p.GetYaw() - 0.7) <= kEps);

    Point6 q(1.0, 1.0, 1.0, 0.1, 0.1, 0.1);
    Point6 sum = p + q;
    Point6 diff = p - q;

    // Verify + and - operators.
    assert(std::abs(sum.GetX() - 4.0) <= kEps);
    assert(std::abs(diff.GetX() - 2.0) <= kEps);
}

void TestVectorOpsAndString()
{
    Point6 a(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);

    std::string s = a.ToString();
    // Verify string conversion is non-empty and formatted.
    assert(!s.empty());
    assert(s.front() == '(');
    assert(s.back() == ')');
}
} // namespace

int main()
{
    TestConstructAndAccessors();
    TestIndexOperator();
    TestTranslateAndArithmetic();
    TestVectorOpsAndString();
    return 0;
}
