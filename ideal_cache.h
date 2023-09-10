#ifndef IDEAL_CACHE_H
#define IDEAL_CACHE_H
#include <unordered_set>

namespace caches {
template<typename KeyT>
class IdealCache {
    size_t hits{0};
    std::unordered_set<KeyT> m_cache;    
public:
    IdealCache() {}
    size_t getHits() const {return hits;}
    void lookup_update(KeyT key) {
        if(m_cache.find(key) != m_cache.end())
            hits++;
        m_cache.emplace(key);
    }
};
}
#endif 