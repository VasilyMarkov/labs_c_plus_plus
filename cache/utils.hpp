#pragma once
#include <iostream>
#include <vector>
#include <exception>
#include <iterator>

template <typename T>
void print(const std::vector<T>& vec) {
    std::copy(std::begin(vec), std::end(vec), std::ostream_iterator<T>(std::cout, ", "));
    std::cout << std::endl;
}

std::pair<size_t, std::vector<int>> readInputData() {
    std::vector<int> pages;
    size_t m, n;
    std::cin >> m >> n;

    for(size_t i = 0; i < n; ++i) {
        int tmp;
        std::cin >> tmp;
        pages.push_back(tmp);
    }
    if(std::cin.fail()) throw std::runtime_error("Invalid input data");
    return {m, pages};
}
