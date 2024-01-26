#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <optional>
#include "linal.hpp"
// #define TEST

template <typename T>
void print(const std::vector<T>& vec) {
    for(auto const& i : vec) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
void print(const std::set<T>& set) {
    for(auto const& i : set) {
        std::cout << i << std::endl;
    }
}

template <typename T, typename U>
void print(const std::multimap<T, U>& vec) {
    for(auto& [key, value] : vec) {
        std::cout << key << ' ' << value << std::endl;
    }
}

int main() {

#ifndef TEST
    size_t N{0};
    std::cin >> N;
    std::vector<double> points(N*9);
    for(auto i = 0; i < N*9; ++i) {
        auto tmp{0.0};
        std::cin >> tmp;
        points[i] = tmp;
    }
    const auto triangles = createTriangles(points);
    const auto result = intersectTriangles(triangles);
    if(result != std::nullopt) {
        print(result.value());
    }
#else
    std::vector<int> points = {0,0,0, 0,1,0, 1,0,0,
                               0,0,0, 0,2,0, 2,0,0};
    const auto triangles = createTriangles(points);
    const auto result = intersectTriangles(triangles);
    if(result != std::nullopt) {
        print(result.value());
    }
    else std::cout << "nullopt" << std::endl;

#endif
    return 0;
}
