#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "arc.h"
#include "ideal_cache.h"

void createRandomData(std::vector<int>& pages, size_t num, size_t mode)
{
    pages.clear();

    std::random_device rd;
    std::mt19937 gen(rd());
    if (mode == 0) {
        std::uniform_int_distribution<> distrib(1, num);
        pages.resize(num);
        for(auto i =0 ; i < pages.size(); ++i) {
            pages[i] = distrib(gen);
        }
    }
    else if (mode == 1) {
        std::normal_distribution<> distrib(num/2, num/3);
        auto random_int = [&distrib, &gen]{ return std::round(distrib(gen)); };
        pages.resize(num);
        for(auto i =0 ; i < pages.size(); ++i) {
            pages[i] = random_int();
        }
    }
    else if (mode == 2) {
        std::chi_squared_distribution<> distrib(num/3);
        pages.resize(num);
        for(auto i =0 ; i < pages.size(); ++i) {
            pages[i] = distrib(gen);
        }
    }
    else if (mode == 3) {
        std::lognormal_distribution<> distrib(log(num/2), log(num/3));
        pages.resize(num);
        for(auto i =0 ; i < pages.size(); ++i) {
            pages[i] = distrib(gen);
        }
    }

}

void print(const std::vector<int>& data) {
    for(const auto& i : data) {
        std::cout << i <<  ' ';

    }
    std::cout << std::endl;
}

using namespace std::chrono;
int main()
{
    std::vector<int> pages;

    size_t m, n;
    std::cin >> m >> n;
    for(size_t i = 0; i < n; ++i) {
        int tmp;
        std::cin >> tmp;
        pages.push_back(tmp);
    }


#ifdef ARC_DEF
    caches::ARC<int> arc{m};
    auto test {[&](std::vector<int>& pages){
        print(pages);
        for(const auto& i : pages) {
            arc.lookup_update(i);
        }
        std::cout << arc.getHits() << std::endl;

        arc.clear();
    }};
    test(pages);
#else
    caches::IdealCache<int> ideal{m};
    ideal.setBuffer(pages);
    for(const auto& i : pages) {
        ideal.lookup_update(i);
    }
    std::cout << ideal.getHits() << std::endl;
#endif
    return 0;
}
