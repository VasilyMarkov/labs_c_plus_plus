#ifndef CACHE_H
#define CACHE_H
#include <iostream>
#include <list>
#include <unordered_map>

namespace my {

template <typename T, typename KeyT = int>
class Cache{
private:
    size_t m_size;

    std::list<std::pair<KeyT, T>> m_cache;
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;

    std::unordered_map<KeyT, ListIt> m_hash;
public:
    Cache(size_t size): m_size(size) {}

    bool full() const {return (m_cache.size() == m_size);}

    template<typename F_ptr>
    bool lookup_update(int key, F_ptr slow_get_page)
    {
        auto hit = m_hash.find(key);
        if(hit == m_hash.end()) {
            if (full()) {
                m_hash.erase(m_cache.back().first);
                m_cache.pop_back();
            }

            m_cache.emplace_front(key, slow_get_page(key));
            m_hash.emplace(key, std::begin(m_cache));
            return false;
        }
        else {
            auto eltit = hit->second;
            if(eltit != std::begin(m_cache))
                m_cache.splice(std::begin(m_cache), m_cache, eltit, std::next(eltit));
            return true;
        }
    }
};

}



#endif // CACHE_H
