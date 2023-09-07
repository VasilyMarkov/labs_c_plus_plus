#include <iostream>
#include <vector>
#include "arc.h"
#include "ideal_cache.h"

int main()
{
    size_t m = 5;
    caches::ARC<int> arc{m};
    caches::IdealCache<int> ideal;
    std::vector<int> pages;

    auto test {[&](std::vector<int>& pages){
        for(const auto& i : pages) {
            arc.lookup_update(i);
        }
        arc.print();
        arc.clear();
    }};

    pages = {1, 2, 3, 4, 5, 6};
    test(pages);

    pages = {1, 2, 3, 4, 5, 1};
    test(pages);

    arc.setSize(4);
    pages = {1, 2, 3, 4, 5, 1};
    test(pages);

    arc.setSize(7);
    pages = {1, 2, 3, 4, 1, 1, 2};
    test(pages);

    arc.setSize(8);
    pages = {1, 1, 1, 1, 2, 3, 4, 5, 6, 6, 6, 6, 6, 1, 1};
    test(pages);
    for(const auto& i : pages) {
        ideal.lookup_update(i);
    }
    std::cout << "Ideal cache hits: " << ideal.getHits() << std::endl;

    return 0;
}
