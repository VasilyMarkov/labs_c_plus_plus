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
//    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
//                               -1,-1,0, -1,4,0, 4,-1,0};
//    auto triangles = createTriangles(points);
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    auto result = intersectTriangles(triangles);
//    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
//    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;


    size_t N{0};
    std::cin >> N;
    std::vector<double> points(N);
    for(auto i = 0; i < N; ++i) {
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
