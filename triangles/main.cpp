#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>

#define MY

#ifdef MY
#include "linal.hpp"
#else
#include "lingeo.hpp"
#endif

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
#ifndef MY
polygon_t input_triangle() {
  std::vector<point_t> verts(3);
  point_t temp;
  for (int i = 0; i < 3; i++) {
    std::cin >> temp.x;
    std::cin >> temp.y;
    verts[i] = temp;
    assert(std::cin.good());
  }
  polygon_t ret{verts};
  return ret;
}
#endif

int main() {
#ifdef MY
    std::vector<int> points = {0,0,0, 0,1,0, 0,0,1,
                               0,0,2, 1,0,2, 1,0,3};
//    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
//                               -1,-1,0, -1,3,0, 3,-1,0};
    auto triangles = createTriangles(points);
    auto result = triangles.at(0).separable_plane_from(triangles.at(1));
    std::cout << std::boolalpha << result << std::endl;
//    result = triangles.at(1).separable_plane_from(triangles.at(0));
//    std::cout << std::boolalpha << result << std::endl;
//    auto tri = triangles[0].triangleProjection();
//    tri = triangles[1].triangleProjection();
//    auto result = intersectTriangles(triangles);
//    print(result.value());
//    auto result = triangles[0].intersect(triangles[1]);
//    std::cout << std::boolalpha << result << std::endl;
//    result = triangles[1].intersect(triangles[0]);
//    std::cout << std::boolalpha << result << std::endl;
#else
    std::vector<point_t> verts = {{0,0}, {1,0}, {0,1}};
    std::vector<point_t> verts1 = {{0,2}, {1,2}, {0,3}};
    polygon_t triangle1 = polygon_t {verts};
    polygon_t triangle2 = polygon_t {verts1};
    auto result = triangle2.intersect(triangle1);
    std::cout << std::boolalpha << result << std::endl;

    result = triangle1.intersect(triangle2);
    std::cout << std::boolalpha << result << std::endl;

#endif

    return 0;
}
