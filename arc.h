#ifndef CACHE_H
#define CACHE_H
#include <iostream>
#include <list>
#include <unordered_map>

namespace caches {

template <typename T, typename KeyT = int>
class ARC{
private:
    size_t cache_size{0};
    size_t hits{0};
    double p{0};
    using list_t = typename std::list<KeyT>;
    using list_it = typename std::list<KeyT>::iterator;
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

    void move(buffer& src, buffer& dst, KeyT key) {
        src.list.erase(src.hash[key]);

        if(full(dst.list)) {
            dst.list.pop_back();
        }
        dst.list.emplace_front(key);

    }
    void toGhost(const KeyT i, const double p) {
        if(!mru.list.empty() && ((mru.list.size() > p) || ((mfu_ghost.hash.find(i) != mfu_ghost.hash.end()) && (p == mru.list.size())))) {
            mru_ghost.hash.emplace(mru.list.back(), mru.list.begin());
            auto lru = mru.list.back();
            move(mru, mru_ghost, lru);
            mru.hash.erase(lru);
        }
        else {
            mfu_ghost.hash.emplace(mfu.list.back(), mfu.list.begin());
            auto lfu = mfu.list.back();
            move(mfu, mfu_ghost, lfu);
            mfu.hash.erase(lfu);
        }
    }

public:
    ARC():cache_size(0) {}
    ARC(size_t size): cache_size(size) {}
    void setSize(size_t size) {cache_size = size;}
    size_t getHits() const {return hits;}
    bool full(const list_t& list) const {return (list.size() == cache_size);}

    void lookup_update(int key)
    {
        if(mru.hash.find(key) != mru.hash.end()) {
            hits++;
            move(mru, mfu, key);
            mfu.hash.emplace(key, mfu.list.begin());
            mru.hash.erase(key);
        }
        else if(mfu.hash.find(key) != mfu.hash.end()) {
            hits++;
            move(mfu, mfu, key);
        }
        else if(mru_ghost.hash.find(key) != mru_ghost.hash.end()) {
            hits++;
            p = std::min(static_cast<double>(cache_size), p+std::max(static_cast<double>(mfu_ghost.list.size())/mru_ghost.list.size(), 1.0));
            toGhost(key, p);
            move(mru_ghost, mfu, key);
        }
        else if(mfu_ghost.hash.find(key) != mfu_ghost.hash.end()) {
            hits++;
            p = std::min(static_cast<double>(cache_size), p-std::max(static_cast<double>(mfu_ghost.list.size())/mru_ghost.list.size(), 1.0));
            toGhost(key, p);
            move(mfu_ghost, mfu, key);
        }
        else {
            if(mru.list.size() + mru_ghost.list.size() == cache_size) {
                if(mru.list.size() < cache_size) {
                    mru_ghost.hash.erase(mru_ghost.list.back());
                    mru_ghost.list.pop_back();
                    toGhost(key, p);
                }
                else {
                    mru.hash.erase(mru.list.back());
                    mru.list.pop_back();
                }
            }
            else if(mru.list.size() + mru_ghost.list.size() < cache_size) {
                if(mru.list.size()+mfu.list.size()+mru_ghost.list.size()+mfu_ghost.list.size() >= cache_size) {
                    if(mru.list.size()+mfu.list.size()+mru_ghost.list.size()+mfu_ghost.list.size() == 2*cache_size) {
                        mfu_ghost.list.pop_back();
                    }
                    toGhost(key, p);
                }
            }
            mru.list.emplace_front(key);
            mru.hash.emplace(key, mru.list.begin());
        }
    }

    void clear() {
        mru.clear();
        mfu.clear();
        mru_ghost.clear();
        mfu_ghost.clear();
        hits = 0;
    }

    void print() const {
        std::cout << "Hits: " << hits << std::endl;
        std::cout << "MRU size: " << mru.list.size() << std::endl;
        std::cout << "MFU size: " << mfu.list.size() << std::endl;
        std::cout << "MRU ghost size: " << mfu_ghost.list.size() << std::endl;
        std::cout << "MFU ghost size: " << mru_ghost.list.size() << std::endl;
        std::cout << "p : " << p << std::endl;
        std::cout << "==========================================" << std::endl;
    }
};

}



#endif // CACHE_H
