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

struct Point {
    double x_;
    double y_;
    double z_;
    Point(double x, double y, double z):x_(x), y_(y), z_(z) {}
};

int sign_of_determinant(Point a, Point b, Point c, Point d) {
    auto det = (a.x_-b.x_)*(c.y_*d.z_-c.z_*d.y_)+(b.y_-a.y_)*(c.x_*d.z_-c.z_*d.x_)+(a.z_-b.z_)*(c.x_*d.y_-c.y_*d.x_)-
                a.x_*(b.y_*d.z_-b.z_*d.y_)+a.y_*(b.x_*d.z_-b.z_*d.x_)-a.z_*(b.x_*d.y_-b.y_*d.x_)+
                a.x_*(b.y_*c.z_-b.z_*c.y_)-a.y_*(b.x_*c.z_-b.z_*c.x_)+a.z_*(b.x_*c.y_-b.y_*c.x_);
    return det;
}

enum class position {
    same_half_space,
    diff_half_space,
    same_plane
};

position checkRelativePosition(const std::vector<int>& dets) {
    if(std::all_of(std::begin(dets), std::end(dets), [](int i) { return i==0; })) {
        return position::same_plane;
    }
    else {
        bool same_sign = false;
        for(auto i = 1; i < dets.size(); ++i) {
            if(!(dets[i-1] >= 0) ^ (dets[i] < 0)) { //it's a diff sign?
                return position::diff_half_space;
            }
        }
//        std::all_of(dets.begin(), dets.end(),[sign = dets.front() >= 0](int item){return sign == item >= 0;});
        return position::same_half_space;
    }
}

int main() {
#ifdef MY
    std::vector<int> points = {0,0,0, 0,1,0, 1,0,0,
                               0,0,0, 0,-1,0, -1,0,0};
    auto triangles = createTriangles(points);
    auto result = triangles.at(0).separable_plane_from(triangles.at(1));

    std::cout << std::boolalpha << result << std::endl;
#else
//    std::vector<point_t> verts = {{0,0}, {1,0}, {0,1}};
//    std::vector<point_t> verts1 = {{0,0}, {-1,0}, {0,-1}};

    std::vector<point_t> verts = {{0,0}, {1,0}, {0,1}};
    std::vector<point_t> verts1 = {{0,0}, {-1,0}, {0,-1}};

    polygon_t triangle1 = polygon_t {verts};
    polygon_t triangle2 = polygon_t {verts1};
    auto result = triangle2.intersect(triangle1);
    std::cout << std::boolalpha << result << std::endl;


//    result = triangle1.separable_from(triangle2);
//    std::cout << std::boolalpha << result << std::endl;

#endif
    return 0;
}
