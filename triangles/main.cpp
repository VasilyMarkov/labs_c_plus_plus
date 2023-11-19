#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>
#include "linal.hpp"

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
    std::vector<int> points = {0,0,0, 0,1,0, 1,0,0,
                               0,0,0, 0,2,0, 2,0,0};
//    std::vector<int> points = {0,0,0, 0,-1,0, -1,0,0,
//                               0,0,0, 0,2,0, 2,0,0};
    auto triangles = createTriangles(points);
    auto sep = triangles.at(0).separable_line_from(triangles.at(1));
//    auto result = intersectTriangles(triangles);
    return 0;
}
