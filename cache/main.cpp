#include <iostream>
#include <vector>
#include <random>
#include <exception>
#include "arc.h"
#include "ideal_cache.h"

void print(const std::vector<int>& data) {
    for(const auto& i : data) {
        std::cout << i <<  ' ';

    }
    std::cout << std::endl;
}

int slow_get_page_int(int key) { return key; }

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
    try {
        caches::ARC<int> arc{m};
        auto test {[&](std::vector<int>& pages){
            for(const auto& i : pages) {
                arc.lookup_update(i, slow_get_page_int);
            }
            std::cout << arc.getHits() << std::endl;

            arc.clear();
        }};
        test(pages);
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
#else
    caches::IdealCache<int> ideal{m};
    ideal.setBuffer(pages);
    for(const auto& i : pages) {
        ideal.lookup_update(i, slow_get_page_int);
    }
    std::cout << ideal.getHits() << std::endl;
#endif
    return 0;
}
