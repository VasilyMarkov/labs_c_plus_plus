#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <chrono>
#include "linal.hpp"

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
    size_t N{0};
    std::cin >> N;
    std::vector<double> points(N*9);
    for(auto i = 0; i < N*9; ++i) {
        auto tmp{0};
        std::cin >> tmp;
        points[i] = tmp;
    }
    auto triangles = createTriangles(points);
    auto result = intersectTriangles(triangles);
    if(result != std::nullopt) {
        print(result.value());
    }
    return 0;
}
