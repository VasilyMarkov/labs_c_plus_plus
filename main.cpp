#include <iostream>
#include "cache.h"
#include <vector>

int slow_get_page_int(int key) {return key;}

int main()
{
    size_t hits{0};
    size_t m = 4, n = 0;
    caches::Cache<int> c{m};
    std::vector<int> indices = {1, 2, 1, 4, 5, 6, 7, 8, 2, 4, 3, 4};
    for(const auto& i : indices) {
        c.lookup_update(i);
    }
    std::cout << c.getHits() << std::endl;
    return 0;
}
