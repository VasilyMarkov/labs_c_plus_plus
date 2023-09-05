#include <iostream>
#include "cache.h"
#include <vector>

int slow_get_page_int(int key) {return key;}

int main()
{
    size_t hits{0};
    size_t m = 4, n = 0;

    my::Cache<int> c{m};
    std::vector<int> indices = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
//    for(auto i = 0; i < n; ++i) {
//        int tmp;
//        std::cin >> tmp;
//        if(c.lookup_update(tmp, slow_get_page_int)) hits++;
//    }
    for(const auto& i : indices) {
        if(c.lookup_update(i, slow_get_page_int)) hits++;
    }
    std::cout << hits << std::endl;
    return 0;
}
