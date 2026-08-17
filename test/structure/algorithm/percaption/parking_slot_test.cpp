#include <cassert>
#include <cmath>

#include "../../../../structure/algorithm/perception/parking_slot.h"

namespace
{
using Slot = structure::perception::ParkingSlot<double>;
using TS = structure::Timestamp;
using Center = Slot::Center;
using Vertices = Slot::Vertices;
using EntranceVertices = Slot::EntranceVertices;
using EntranceVertexIndices = Slot::EntranceVertexIndices;
using ParkingSlotGeometryType = structure::perception::ParkingSlotGeometryType;
using ParkingSlotPhysicalType = structure::perception::ParkingSlotPhysicalType;

Vertices MakeRectangleVertices(double x0, double y0, double x1, double y1)
{
    Vertices vertices{};
    vertices[0] = Slot::Vertex(x0, y0, 0.0);
    vertices[1] = Slot::Vertex(x1, y0, 0.0);
    vertices[2] = Slot::Vertex(x1, y1, 0.0);
    vertices[3] = Slot::Vertex(x0, y1, 0.0);
    return vertices;
}

void TestConstructorAndGetters()
{
    const TS timestamp = TS::FromSeconds(10);
    const Vertices vertices = MakeRectangleVertices(0.0, 0.0, 2.0, 5.0);
    const Center center(1.0, 2.5, 0.0);

    Slot slot(timestamp,
              42,
              ParkingSlotGeometryType::PARALLEL_2,
              ParkingSlotPhysicalType::COMMON_1,
              center,
              vertices);

    assert(slot.GetTimestamp().Nanoseconds() == TS::FromSeconds(10).Nanoseconds());
    assert(slot.GetID() == 42);
    assert(slot.GetGeometryType() == ParkingSlotGeometryType::PARALLEL_2);
    assert(slot.GetPhysicalType() == ParkingSlotPhysicalType::COMMON_1);
    assert(std::abs(slot.GetCenter().GetX() - 1.0) < 1e-12);
    assert(std::abs(slot.GetCenter().GetY() - 2.5) < 1e-12);
}

void TestEntrance()
{
    Slot slot;
    const EntranceVertices entrance_vertices{
        Slot::Vertex(0.0, 0.0, 0.0),
        Slot::Vertex(2.0, 0.0, 0.0)};
    const EntranceVertexIndices entrance_indices{0, 1};

    slot.SetEntranceVertices(entrance_vertices);
    slot.SetEntranceVertexIndices(entrance_indices);

    assert(std::abs(slot.GetEntranceVertices()[0].GetX() - 0.0) < 1e-12);
    assert(std::abs(slot.GetEntranceVertices()[1].GetX() - 2.0) < 1e-12);
    assert(slot.GetEntranceVertexIndices()[0] == 0);
    assert(slot.GetEntranceVertexIndices()[1] == 1);

    slot.GetEntranceVertices()[0].SetX(-1.0);
    slot.GetEntranceVertexIndices()[1] = 3;
    assert(std::abs(slot.GetEntranceVertices()[0].GetX() + 1.0) < 1e-12);
    assert(slot.GetEntranceVertexIndices()[1] == 3);
}

void TestGeometryMetrics()
{
    Slot slot;
    slot.SetVertices(MakeRectangleVertices(0.0, 0.0, 2.0, 5.0));

    assert(slot.HasValidGeometry());
    assert(std::abs(slot.GetArea2D() - 10.0) < 1e-12);
    assert(std::abs(slot.GetLength2D() - 5.0) < 1e-12);
    assert(std::abs(slot.GetWidth2D() - 2.0) < 1e-12);
}

void TestSettersAndState()
{
    Slot slot;

    slot.SetTimestamp(TS::FromMilliseconds(123));
    slot.SetID(7);
    slot.SetGeometryType(ParkingSlotGeometryType::PERPENDICULAR_1);
    slot.SetPhysicalType(ParkingSlotPhysicalType::MECHANICAL_3);
    slot.SetVertices(MakeRectangleVertices(2.0, 2.0, 4.0, 6.0));
    slot.SetVertex(0, Slot::Vertex(4.0, 4.0, 2.0));
    slot.SetAvailable(true);

    assert(slot.GetTimestamp().Nanoseconds() == TS::FromMilliseconds(123).Nanoseconds());
    assert(slot.GetID() == 7);
    assert(slot.GetGeometryType() == ParkingSlotGeometryType::PERPENDICULAR_1);
    assert(slot.GetPhysicalType() == ParkingSlotPhysicalType::MECHANICAL_3);
    assert(std::abs(slot.GetCenter().GetX() - 3.5) < 1e-12);
    assert(std::abs(slot.GetCenter().GetY() - 4.5) < 1e-12);
    assert(std::abs(slot.GetCenter().GetZ() - 0.5) < 1e-12);
    assert(slot.IsAvailable());
}

void TestConfidenceClamp()
{
    Slot slot;

    slot.SetConfidence(-1.0);
    assert(std::abs(slot.GetConfidence() - 0.0) < 1e-12);

    slot.SetConfidence(0.5);
    assert(std::abs(slot.GetConfidence() - 0.5) < 1e-12);

    slot.SetConfidence(3.0);
    assert(std::abs(slot.GetConfidence() - 1.0) < 1e-12);
}
} // namespace

int main()
{
    TestConstructorAndGetters();
    TestEntrance();
    TestGeometryMetrics();
    TestSettersAndState();
    TestConfidenceClamp();
    return 0;
}