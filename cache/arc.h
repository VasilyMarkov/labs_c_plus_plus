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
    double p{0}; //balance target
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
        try {
            dst.list.emplace_front(key, slow_get_page(key));
            dst.hash.emplace(key, dst.list.begin());
            src.list.erase(src.hash[key]);
            src.hash.erase(key);
        }
        catch(std::exception& e) {
            dst.list.pop_front();
            throw std::runtime_error("Move failed");
        }
    }

    template <typename F> void moveTop(buffer& src, KeyT key, F slow_get_page) {
        try {
            src.list.emplace_front(key, slow_get_page(key));
            src.list.erase(src.hash[key]);
            src.hash[key] = src.list.begin();
        }
        catch(std::exception& e) {
            src.list.pop_front();
            throw std::runtime_error("MoveTop failed");
        }
    }

    template <typename F> void toGhost(const KeyT i, const double p, F slow_get_page) {
        if(!mru.list.empty() && ((mru.list.size() > p) || ((mfu_ghost.hash.find(i) != mfu_ghost.hash.end()) && (p == mru.list.size())))) {
            move(mru, mru_ghost, mru.list.back().first, slow_get_page);
            return;
        }
        move(mfu, mfu_ghost, mfu.list.back().first, slow_get_page);
    }
    bool full(const list_t& list) const noexcept {return (list.size() == cache_size);}
public:
    ARC() = default;
    ARC(size_t size): cache_size(size/2) {
        if (size < 1) throw std::runtime_error("Wrong cache size"); 
        if (cache_size % 2 != 0) odd_size = true;
    }
    void setSize(size_t size) {cache_size = size/2; if (cache_size % 2 != 0) odd_size = true;}
    size_t getHits() const {return hits;}
    template <typename F> void lookup_update(int key, F slow_get_page)
    {
        if(mru.hash.find(key) != mru.hash.end()) {
            move(mru, mfu, key, slow_get_page);
            hits++;
        }
        else if(mfu.hash.find(key) != mfu.hash.end()) {  
            moveTop(mfu, key, slow_get_page);
            hits++;
        }
        else if(mru_ghost.hash.find(key) != mru_ghost.hash.end()) {
            try {
                p = std::min(static_cast<double>(cache_size), p + std::max(static_cast<double>(mfu_ghost.list.size()) / mru_ghost.list.size(), 1.0));
                toGhost(key, p, slow_get_page);
                move(mru_ghost, mfu, key, slow_get_page);
                hits++;
            }
            catch(std::runtime_error& e) {
                p = std::min(static_cast<double>(cache_size), p + std::max(static_cast<double>(mfu_ghost.list.size()) / mru_ghost.list.size(), 1.0));
            }
        }
        else if(mfu_ghost.hash.find(key) != mfu_ghost.hash.end()) {
            try {
                p = std::max(0.0, p - std::max(static_cast<double>(mru_ghost.list.size()) / mfu_ghost.list.size(), 1.0));
                toGhost(key, p, slow_get_page);
                move(mfu_ghost, mfu, key, slow_get_page);
                hits++;
            }
            catch(std::runtime_error& e) {
                p = std::min(static_cast<double>(cache_size), p + std::max(static_cast<double>(mfu_ghost.list.size()) / mru_ghost.list.size(), 1.0));
            }
        }
        else {
            if(mru.list.size() + mru_ghost.list.size() == cache_size) {
                if(mru.list.size() < cache_size) {
                    auto key_backup = mru_ghost.list.back().first;
                    auto it_backup = mru_ghost.list.end();
                    mru_ghost.hash.erase(mru_ghost.list.back().first);
                    mru_ghost.list.pop_back();
                    try {
                        toGhost(key, p, slow_get_page);
                    }
                    catch (std::runtime_error& e) {
                        mru_ghost.hash.emplace(key_backup, it_backup);
                        mru_ghost.list.emplace_back(key_backup, slow_get_page(key_backup));      
                    }
                }
                else {
                    mru.hash.erase(mru.list.back().first);
                    mru.list.pop_back();
                }
            }
            else if(mru.list.size() + mru_ghost.list.size() < cache_size) {
                if(mru.list.size() + mfu.list.size() + mru_ghost.list.size() + mfu_ghost.list.size() >= cache_size) {
                    toGhost(key, p, slow_get_page);

                    auto size_limit = 2*cache_size;
                    if (odd_size) size_limit++;
                    if(mru.list.size() + mfu.list.size() + mru_ghost.list.size() + mfu_ghost.list.size() == size_limit) {
                        mfu_ghost.hash.erase(mfu_ghost.list.back().first);
                        mfu_ghost.list.pop_back();
                    }      
                }
            }
            try {
                mru.list.emplace_front(key, slow_get_page(key));
                mru.hash.emplace(key, mru.list.begin());
            }
            catch (std::runtime_error&) {
                mru.list.pop_front();
            }
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
