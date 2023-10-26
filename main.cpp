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

int slow_get_page_int(int key) { return key; }
//#define LOOP

int main()
{
    std::vector<int> pages;
#ifndef LOOP
    size_t m, n;
    std::cin >> m >> n;
    for(size_t i = 0; i < n; ++i) {
        int tmp;
        std::cin >> tmp;
        pages.push_back(tmp);
    }

#else
    size_t m = 20;
#endif

#ifdef ARC_DEF
    #ifndef LOOP
    caches::ARC<int> arc{m};
    auto test {[&](std::vector<int>& pages){
        for(const auto& i : pages) {
            arc.lookup_update(i, slow_get_page_int);
        }
        std::cout << arc.getHits() << std::endl;

        arc.clear();
    }};
    test(pages);
    #else
        for(auto i = 0; i < 10000; ++i) {
            std::vector<int> pages;
            createRandomData(pages, m*4, 3);
            caches::ARC<int> arc(m);
            caches::IdealCache<int> ideal{m};
            ideal.setBuffer(pages);
//            print(pages);
            for(const auto& i : pages) {
                arc.lookup_update(i);
                ideal.lookup_update(i);
            }

//            std::cout << arc.getHits() << ' ' << ideal.getHits() << std::endl;
            if(arc.getHits() > ideal.getHits()) print(pages);
        }
        std::cout << "End" << std::endl;
    #endif
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
