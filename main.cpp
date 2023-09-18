#include <iostream>
#include <vector>
#include "arc.h"
#include "ideal_cache.h"
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
        for(const auto& i : pages) {
            arc.lookup_update(i);
        }
        std::cout << arc.getHits() << std::endl;
        arc.clear();
    }};
    test(pages);
#else
    caches::IdealCache<int> ideal;
    for(const auto& i : pages) {
        ideal.lookup_update(i);
    }
    std::cout << ideal.getHits() << std::endl;
#endif
    return 0;
}
