#include "utils.hpp"
#include "arc.h"

int slow_get_page_int(int key) { return key; }

int main()
{
    try {
        auto [cache_size, pages] = readInputData();
        caches::ARC<int> arc{cache_size};

        for(const auto& i : pages) {
            arc.lookup_update(i, slow_get_page_int);
        }
        std::cout << arc.getHits() << std::endl;

        arc.clear();
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
