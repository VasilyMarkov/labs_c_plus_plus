#ifndef IDEAL_CACHE_H
#define IDEAL_CACHE_H
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <map>

namespace caches {
template<typename KeyT = int>
class IdealCache {
    size_t hits{0};
    size_t cache_size{0};
    std::unordered_set<KeyT> hash;
    std::map<KeyT, std::list<size_t>*> input_hash;
    std::vector<KeyT> input_buffer;
    std::map<size_t, KeyT> cache_map;

    bool full() const {return (cache_map.size() == cache_size);}

    void initFutureHash() {
        for(auto i = 0; i < input_buffer.size(); ++i) {
            if(input_hash.find(input_buffer[i]) == input_hash.end()) {
                input_hash.emplace(input_buffer[i], new std::list<size_t>(1, i));
            }
            else {
                input_hash.at(input_buffer[i])->emplace_back(i);
            }
        }
    }

    void updateFutureHash(KeyT key) {
        input_hash.at(key)->pop_front();
        if(input_hash.at(input_buffer.front())->empty()) {
            input_hash.erase(input_buffer.front());
        }
        input_buffer.erase(std::begin(input_buffer));
    }

public:
    IdealCache() = default;
    IdealCache(size_t size): cache_size(2*size) {}
    IdealCache(const std::vector<KeyT>& buffer): input_buffer(buffer) {}

    void setBuffer(std::vector<KeyT>& buffer) {
        input_buffer.clear();
        input_buffer = buffer;
        initFutureHash();
    }
    size_t getHits() const {return hits;}

    void setSize(size_t size) {
        cache_size = 2*size;
    }

    void lookup_update(KeyT key) {
        if(input_buffer.empty()) return;        
        auto key_index = input_hash.at(key)->front();
        updateFutureHash(key);
        if(hash.find(key) != hash.end()) {
            hits++;
            cache_map.erase(key_index);
            if(input_hash.find(key) == input_hash.end())
                hash.erase(key);

            else {
                cache_map[input_hash.at(key)->front()] = key;
            }
        }
        if(full()) {
            if(hash.find(key) == hash.end()) {
                if(input_hash.find(key) == input_hash.end()) {
                    return;
                }
                auto max_index = cache_map.rbegin();
                hash.erase(max_index->second);
                cache_map.erase(max_index->first);
                cache_map.emplace(input_hash.at(key)->front(), key);
                hash.emplace(key);

            }
        }
        else {
            if(hash.find(key) == hash.end() && input_hash.find(key) != input_hash.end()) {
                hash.emplace(key);
                cache_map[input_hash.at(key)->front()] = key;
            }
        }
        return;
    }
};
}
#endif 
