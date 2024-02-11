#include "ideal_cache.h"
#include "utils.hpp"

int slow_get_page_int(int key) { return key; }

int main()
{
    try {
        auto [cache_size, pages] = readInputData();
        caches::IdealCache<int> ideal{cache_size};
        ideal.setBuffer(pages);
        
        for(const auto& i : pages) {
            ideal.lookup_update(i, slow_get_page_int);
        }
        std::cout << ideal.getHits() << std::endl;
    }
    catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
