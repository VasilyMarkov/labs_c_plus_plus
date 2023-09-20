#ifndef IDEAL_CACHE_H
#define IDEAL_CACHE_H
#include <unordered_set>
#include <iostream>
#include <vector>
#include <queue>
#include <list>

namespace caches {
template<typename KeyT = int>
class IdealCache {
    size_t hits{0};
    size_t cache_size{0};
    std::unordered_set<KeyT> hash;
    std::unordered_set<KeyT> window_hash;
    std::list<KeyT> cache;
    std::vector<KeyT> input_buffer;
    std::vector<KeyT> future_window; //Contains future elements
    bool full(const std::list<KeyT>& list) const {return (list.size() == cache_size);}
    void updateWindow() {
        future_window = {std::begin(input_buffer), std::begin(input_buffer)+cache_size};
        window_hash = {std::begin(input_buffer), std::begin(input_buffer)+cache_size};
    }
public:
    IdealCache():cache_size(0) {}
    IdealCache(const std::vector<KeyT>& buffer): input_buffer(buffer) {
    }
    void setBuffer(std::vector<KeyT>& buffer) {
        input_buffer = buffer;
        updateWindow();
    }
    size_t getHits() const {return hits;}
    void setSize(size_t size) {
        cache_size = size;
        future_window.resize(cache_size);
    }
    template<typename F>
    KeyT lookup_update(KeyT key, F slow_get_page) {
        KeyT page;
        if(full(cache)) {
            if(hash.find(key) != hash.end()) {
                hits++;
                page = slow_get_page(key);
            }
            else {
                for(auto it = cache.begin(); it != cache.end(); ++it) { //Replace if future_window doesn't contain key
                    if(window_hash.find(*it) == window_hash.end()) {
                        hash.erase(*it);
                        *it = key;
                        hash.emplace(key);
                    }
                }
            }
        }
        else {
            cache.emplace_back(key);
            hash.emplace(key);
        }
        input_buffer.erase(std::begin(input_buffer));
        updateWindow();
        return page;
    }
};
}
#endif 
