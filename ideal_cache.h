#ifndef IDEAL_CACHE_H
#define IDEAL_CACHE_H
#include <unordered_set>
#include <iostream>
#include <vector>
#include <queue>
#include <list>

namespace caches {
template<typename KeyT>
class IdealCache {
    size_t hits{0};
    size_t cache_size{0};
    std::unordered_set<KeyT> hash;
    std::unordered_set<KeyT> window_hash;
    std::list<KeyT> cache;
    std::vector<KeyT> input_buffer;
    std::vector<KeyT> input_window;
    bool full(const std::list<KeyT>& list) const {return (list.size() == cache_size);}
    void moveBuffer(std::vector<double>& src, std::vector<double>& window, size_t step) {
        //std::copy()
    }
public:
    IdealCache():cache_size(0) {}
    IdealCache(const std::vector<KeyT>& buffer): input_buffer(buffer) {
    }
    void setBuffer(const std::vector<KeyT>& buffer) {
        input_buffer(buffer);
        input_window(std::begin(buffer, std::begin(buffer)+5));
    }
    size_t getHits() const {return hits;}
    void setSize(size_t size) {
        cache_size = size;

    }
    void lookup_update(KeyT key) {
        input_buffer.erase(std::begin(input_buffer));
//        input_buffer.push_back(key);
//        input_buffer_hash.emplace(key);
//        if(input_buffer.size() == cache_size+1) {
//            if(full(cache)) {
//                 if(hash.find(input_buffer.front()) != hash.end() ) {
//                    hits++;
//                 }
//                 else {
//                    for(auto it = cache.begin(); it != cache.end(); ++it) {
//                        if(input_buffer_hash.find(*it) != hash.end()) {
//                            cache.erase(it);
//                            cache.insert(it, input_buffer.front());
//                        }
//                    }
//                 }
//            }
//            else {
//                cache.push_back(input_buffer.front());
//            }
//            input_buffer_hash.erase(input_buffer.front());
//            input_buffer.pop_front();
//        }
    }
};
}
#endif 
