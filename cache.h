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
    size_t mru_size, mfu_size;
    std::list<std::pair<KeyT, T>> mru, mfu;
    std::list<std::pair<KeyT, T>> mru_ghost, mfu_ghost;
    using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;

    std::unordered_map<KeyT, ListIt> m_hash;
    std::unordered_map<KeyT, ListIt> m_hash_g;
public:
    Cache(size_t size): m_size(size) {mru_size = m_size; mfu_size = m_size;}

    bool full(const std::unordered_map<KeyT, ListIt>& list, size_t size) const {return (list.size() == size);}

    template<typename F_ptr>
    bool lookup_update(int key, F_ptr slow_get_page)
    {
        auto hit = m_hash.find(key);
        if(hit == std::end(m_hash)) {
            if(full(mru, mru_size)) {
                m_hash_g.emplace(mru.back().first, std::end(mru));
                m_hash.erase(mru.back().first);
                mru_ghost.emplace_front(mru.back());
                mru.pop_back();
            }
            else if(full(mru_ghost, mru_size)) {
                m_hash_g.erase(mru_ghost.back().first);
                mru_ghost.pop_back();
            }
            mru.emplace_front(key, slow_get_page(key));
            m_hash.emplace(key, std::begin(mru));

            return false;
        }
        else {
            auto eltit = hit->second;
            if(eltit != std::begin(mru))
                mru.splice(std::begin(mru), mru, eltit, std::next(eltit));
            return true;
        }
    }
};

}



#endif // CACHE_H
