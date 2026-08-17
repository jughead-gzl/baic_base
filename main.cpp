#include "algorithm\base\math.h"
#include "structure\base\line2d.h"
#include "algorithm\planning\dijkstra.h"
#include "algorithm\planning\a_start.h"

#include <iostream>

int main()
{
    structure::Point2D<double> p1(1.0, 1.0);
    structure::Point2D<double> p2(1.0, 1.0);
   
    if (p1 != p2)
    {
        std::cout << "Points are not equal." << std::endl;
    }
    else
    {
        std::cout << "Points are equal." << std::endl;
    }


    // auto chebyshev_distance = algorithm::DistOfChebyshev(p1, p2);
    // auto manhattan_distance = algorithm::DistOfManhattan(p1, p2);
    // auto euclidean_distance = algorithm::DistOfEuclidean(p1, p2);

    // std::cout << "Chebyshev Distance: " << chebyshev_distance << std::endl;
    // std::cout << "Manhattan Distance: " << manhattan_distance << std::endl;
    // std::cout << "Euclidean Distance: " << euclidean_distance << std::endl;

    // structure::Line2D<double> line1(0.0, 0.0, 3.0, 4.0);
    // std::cout << "Line Start: (" << line1[0].GetX() << ", " << line1[0].GetY() << ")" << std::endl;
    // std::cout << "Line End: (" << line1[1].GetX() << ", " << line1[1].GetY() << ")" << std::endl;
    // std::cout << "Line Length: " << line1.GetLength() << std::endl;
    // std::cout << "Line Slope: " << line1.GetSlope() << std::endl;
    // structure::Line2D<double> line2(0.0, 4.0, 3.0, 0.0);
    // std::cout << "Line2 Start: (" << line2[0].GetX() << ", " << line2[0].GetY() << ")" << std::endl;
    // std::cout << "Line2 End: (" << line2[1].GetX() << ", " << line2[1].GetY() << ")" << std::endl;
    // std::cout << "Line2 Length: " << line2.GetLength() << std::endl;
    // std::cout << "Line2 Slope: " << line2.GetSlope() << std::endl;

    // std::cout << "Is cross : " << line1.IsCross(line2) << std::endl;

    // structure::Line2D<double> line3 = line1;
    // line3.Translate(1.0, 1.0);
    // std::cout << "Is cross : " << line1.IsCross(line3) << std::endl;

    // std::cout << algorithm::DistFromPointToLine(p1, line1) << std::endl;

    // std::vector<std::vector<uint32_t>> graph = {
    //     {0, 1, 2, std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max()},
    //     {1, 0, std::numeric_limits<uint32_t>::max(), 8, std::numeric_limits<uint32_t>::max()},
    //     {2, std::numeric_limits<uint32_t>::max(), 0, 3, std::numeric_limits<uint32_t>::max()},
    //     {std::numeric_limits<uint32_t>::max(), 8, 3, 0, 10},
    //     {std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max(), std::numeric_limits<uint32_t>::max(), 10, 0}
    // };

    // Dijkstra dijkstra;
    // dijkstra.run(graph, 0);

    // auto path = dijkstra.getShortestPath(4);
    // std::cout << "Shortest path from node 0 to node 4: ";
    // for (const auto& node : path)
    // {
    //     std::cout << node << " ";
    // }

    // const uint32_t INFINITY_MAX = std::numeric_limits<uint32_t>::max();
    // AStar astar({
    //     {1, 1, 1, 1, 1},
    //     {2, INFINITY_MAX, INFINITY_MAX, INFINITY_MAX, 1},
    //     {1, 1, 1, INFINITY_MAX, 1},
    //     {INFINITY_MAX, INFINITY_MAX, 1, 1, 1},
    //     {1, 1, 1, INFINITY_MAX, 1}}
    // );
    
    // auto astar_path = astar.GetPath({0, 0}, {4, 0});
    // std::cout << "A* Path: ";
    // std::for_each(astar_path.begin(), astar_path.end(), [](const AStar::MapPoint& point) {
    //     std::cout << "(" << point.first << ", " << point.second << ") ";
    // });
    // std::cout << std::endl;

    return 0;
}