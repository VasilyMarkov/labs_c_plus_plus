#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>
#include "triangles.hpp"

template <typename T>
void print(const std::vector<T>& vec) {
    for(auto const& i : vec) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

template <typename T, typename U>
void print(const std::multimap<T, U>& vec) {
    for(auto& [key, value] : vec) {
        std::cout << key << ' ' << value << std::endl;
    }
}

int main() {
    std::vector<int> triangles = {0,0,0, 3,0,0, 3,3,0, 1,0,0, 4,0,0, 4,3,0};
    std::vector<int> x_axis, y_axis, z_axis;

    std::multimap<int, size_t> x_points, y_points, z_points;

    prep::divideByAxis(triangles, x_axis, y_axis, z_axis);
    prep::sortingPoints(x_axis, y_axis, z_axis, x_points, y_points, z_points);

    std::vector<size_t> x_triangles, y_triangles, z_triangles;
    intersect::intersectByAxis(x_points, x_triangles);
    intersect::intersectByAxis(y_points, y_triangles);
    intersect::intersectByAxis(z_points, z_triangles);

    auto x_y_intersect{intersect::axisesCompare(x_triangles, y_triangles)};
    auto intersect{intersect::axisesCompare(x_y_intersect, z_triangles)};
//    print(x_y_intersect);
    print(intersect);
    return 0;
}
