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
    size_t cnt{0};
    bool check = false;
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
        auto test = src.hash[key];
        src.list.erase(test);

        if(full(dst.list)) {
            dst.hash.erase(dst.list.back());
            dst.list.pop_back();
        }
        if(dst.list.empty()) {
            dst.list.emplace_front(key);
            dst.hash.emplace(key, dst.list.begin());
        }
        else {
            auto old_top_key = dst.list.front();
            dst.list.emplace_front(key);
            dst.hash.emplace(key, dst.list.begin());
            dst.hash.emplace(old_top_key, std::next(dst.list.begin(), 1));
        }
    }
    void toGhost(const KeyT i, const double p) {
        if(!mru.list.empty() && ((mru.list.size() > p) || ((mfu_ghost.hash.find(i) != mfu_ghost.hash.end()) && (p == mru.list.size())))) {
            auto lru = mru.list.back();
            move(mru, mru_ghost, lru);
            mru.hash.erase(lru);
        }
        else {
            auto lfu = mfu.list.back();
            move(mfu, mfu_ghost, lfu);
            mfu.hash.erase(lfu);
        }
    }
    void cacheConsistency() const { //Warning: very slow.
        auto checkCache {[=](const buffer& cache)
            {
                if(cache.list.size() != cache.hash.size()) {
                    std::string error_msg = {"Miss Size. Step: " + std::to_string(cnt)};
                    throw error_msg;
                }
                else {
                    if(!cache.list.empty() && !cache.hash.empty()) {
                        bool find_adr = false;
                        for(const auto& i : cache.hash) {
                            for(auto it = cache.list.begin(); it != cache.list.end(); ++it) {
                                if (i.second == it)
                                    find_adr = true;
                            }
                        }
                        std::string error_msg = {"Hash not equal List. Step: " + std::to_string(cnt)};
                        if(!find_adr)
                            throw error_msg;
                    }
                }
            }
        };
        checkCache(mru);
        checkCache(mfu);
        checkCache(mru_ghost);
        checkCache(mfu_ghost);
    }
    bool full(const list_t& list) const {return (list.size() == cache_size);}
public:
    ARC():cache_size(0) {}
    ARC(size_t size): cache_size(size) {}
    void setSize(size_t size) {cache_size = size;}
    size_t getHits() const {return hits;}
    size_t getCount() const {return cnt;}
    int getP() const {return p;}
    void checkArc(bool flag) {check = flag;}
    bool lookup_update(int key)
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
                mru_ghost.hash.erase(key);
                mfu.hash.emplace(key, mfu.list.begin());
            }
            else if(mfu_ghost.hash.find(key) != mfu_ghost.hash.end()) {
                hits++;
                p = std::min(static_cast<double>(cache_size), p-std::max(static_cast<double>(mru_ghost.list.size())/mfu_ghost.list.size(), 1.0));
                toGhost(key, p);
                move(mfu_ghost, mfu, key);
                mfu_ghost.hash.erase(key);
                mfu.hash.emplace(key, mfu.list.begin());
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
                            mfu_ghost.hash.erase(mfu_ghost.list.back());
                            mfu_ghost.list.pop_back();
                        }
                        toGhost(key, p);
                    }
                }
                mru.list.emplace_front(key);
                mru.hash.emplace(key, mru.list.begin());
            }
            if(check) cacheConsistency();
            cnt++;

//        catch (std::string& e) {
//            std::cout << e << std::endl;
//            return false;
//        }
        return true;
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
