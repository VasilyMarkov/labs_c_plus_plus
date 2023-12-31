#ifndef CACHE_H
#define CACHE_H
#include <iostream>
#include <list>
#include <unordered_map>
#include <algorithm>
namespace caches {

template <typename T, typename KeyT = int>
class ARC{
private:
    size_t cache_size{0};
    size_t hits{0};
    double p{0};
    bool odd_size = false;
    using list_t = typename std::list<std::pair<KeyT, T>>;
    using list_it = typename std::list<std::pair<KeyT, T>>::iterator;
    using hash_t = typename std::unordered_map<KeyT, list_it>;
    struct buffer {
        list_t list;
        hash_t hash;
        void clear() {
            list.clear();
            hash.clear();
        }
    };
    buffer mru, mfu, mru_ghost, mfu_ghost;

    template <typename F> void move(buffer& src, buffer& dst, KeyT key, F slow_get_page) {
        src.list.erase(src.hash[key]);
        src.hash.erase(key);
        dst.list.emplace_front(key, slow_get_page(key));
        dst.hash.emplace(key, dst.list.begin());
    }

    template <typename F> void moveTop(buffer& src, KeyT key, F slow_get_page) {
        src.list.erase(src.hash[key]);
        src.list.emplace_front(key, slow_get_page(key));
        src.hash[key] = src.list.begin();
    }

    template <typename F> void toGhost(const KeyT i, const double p, F slow_get_page) {
        if(!mru.list.empty() && ((mru.list.size() > p) || ((mfu_ghost.hash.find(i) != mfu_ghost.hash.end()) && (p == mru.list.size())))) {
            move(mru, mru_ghost, mru.list.back().first, slow_get_page);
        }
        else {
            move(mfu, mfu_ghost, mfu.list.back().first, slow_get_page);
        }
    }
    bool full(const list_t& list) const {return (list.size() == cache_size);}
public:
    ARC():cache_size(0) {}
    ARC(size_t size): cache_size(size/2) {if (cache_size % 2 != 0) odd_size = true;}
    void setSize(size_t size) {cache_size = size/2; if (cache_size % 2 != 0) odd_size = true;}
    size_t getHits() const {return hits;}
    template <typename F> void lookup_update(int key, F slow_get_page)
    {
        if(mru.hash.find(key) != mru.hash.end()) {
            hits++;
            move(mru, mfu, key, slow_get_page);
        }
        else if(mfu.hash.find(key) != mfu.hash.end()) {
            hits++;
            moveTop(mfu, key, slow_get_page);
        }
        else if(mru_ghost.hash.find(key) != mru_ghost.hash.end()) {
            hits++;
            p = std::min(static_cast<double>(cache_size), p+std::max(static_cast<double>(mfu_ghost.list.size())/mru_ghost.list.size(), 1.0));
            toGhost(key, p, slow_get_page);
            move(mru_ghost, mfu, key, slow_get_page);
        }
        else if(mfu_ghost.hash.find(key) != mfu_ghost.hash.end()) {
            hits++;
            p = std::max(0.0, p-std::max(static_cast<double>(mru_ghost.list.size())/mfu_ghost.list.size(), 1.0));
            toGhost(key, p, slow_get_page);
            move(mfu_ghost, mfu, key, slow_get_page);
        }
        else {
            if(mru.list.size() + mru_ghost.list.size() == cache_size) {
                if(mru.list.size() < cache_size) {
                    mru_ghost.hash.erase(mru_ghost.list.back().first);
                    mru_ghost.list.pop_back();
                    toGhost(key, p, slow_get_page);
                }
                else {
                    mru.hash.erase(mru.list.back().first);
                    mru.list.pop_back();
                }
            }
            else if(mru.list.size() + mru_ghost.list.size() < cache_size) {
                if(mru.list.size()+mfu.list.size()+mru_ghost.list.size()+mfu_ghost.list.size() >= cache_size) {
                    auto size_limit = 2*cache_size;
                    if (odd_size) size_limit++;
                    if(mru.list.size()+mfu.list.size()+mru_ghost.list.size()+mfu_ghost.list.size() == size_limit) {
                        mfu_ghost.hash.erase(mfu_ghost.list.back().first);
                        mfu_ghost.list.pop_back();
                    }
                    toGhost(key, p, slow_get_page);
                }
            }
            mru.list.emplace_front(key, slow_get_page(key));
            mru.hash.emplace(key, mru.list.begin());
        }
    }

    void clear() {
        mru.clear();
        mfu.clear();
        mru_ghost.clear();
        mfu_ghost.clear();
        hits = 0;
        p = 0;
    }
};

}



#endif // CACHE_H
