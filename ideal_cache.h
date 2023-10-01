#ifndef IDEAL_CACHE_H
#define IDEAL_CACHE_H
#include <unordered_set>
#include <unordered_map>
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
    std::unordered_map<KeyT, size_t> input_hash;
    std::list<KeyT> cache;
    std::vector<KeyT> input_buffer, future_window;

    bool full(const std::list<KeyT>& list) const {return (list.size() == cache_size);}
    void updateWindow() {
        future_window = {std::begin(input_buffer), std::begin(input_buffer)+10};
    }
    void updateMap() { //Keys with minimal indexes
        input_hash.clear();
        for(auto i = 0; i < future_window.size(); ++i) {
            if(input_hash.find(future_window[i]) == input_hash.end()) {
                input_hash.emplace(future_window[i], i);
            }
        }
    }
public:
    IdealCache() = default;
    IdealCache(size_t size): cache_size(size) {}
    IdealCache(const std::vector<KeyT>& buffer): input_buffer(buffer) {}

    void setBuffer(std::vector<KeyT>& buffer) {
        input_buffer = buffer;
        updateWindow();
        updateMap();
    }
    size_t getHits() const {return hits;}

    void setSize(size_t size) {
        cache_size = size;
    }

    void lookup_update(KeyT key) {
        if(input_buffer.empty()) return;

        input_buffer.erase(std::begin(input_buffer));
        updateWindow();
        updateMap();
        if(full(cache)) {
            if(hash.find(key) != hash.end()) {
                hits++;
            }
            else {
                if(input_hash.find(key) == input_hash.end()) {
                    return;
                }
                size_t max_index = 0;
                auto max_element_it = std::begin(cache);

                for(auto it = cache.begin(); it != cache.end(); ++it) { //Replacing the farthest key in the input_buffer
                    if(input_hash.find(*it) == input_hash.end()) {
                        hash.erase(*it);
                        *it = key;
                        hash.emplace(key);
                        return;
                    }
                    else {
                        if(input_hash[*it] > max_index) {
                            max_index = input_hash[*it];
                            max_element_it = it;
                        }
                    }
                }
                for (const auto& [lkey, value] : input_hash) {
                    if (value == max_index) {
                        hash.erase(lkey);
                        *max_element_it = key;
                        hash.emplace(key);
                        return;
                    }
                }
            }
        }
        else {
            if(hash.find(key) != hash.end()) {
                hits++;
            }
            else {
                cache.emplace_back(key);
                hash.emplace(key);
            }
        }
        return;
    }
};
}
#endif 
