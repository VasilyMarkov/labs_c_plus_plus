#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ideal_cache.h"
#include "arc.h"
#include <vector>
#include <random>
#include <memory>

namespace my {
namespace project {
namespace {
int slow_get_page_int(int key) { return key; }
class CacheTest : public ::testing::Test {
protected:
    size_t num_pages = 1e5;
    std::vector<int> pages;
    caches::IdealCache<int> ic;
    caches::ARC<int> arc;

    CacheTest() {

    }

    ~CacheTest() override {

    }

    void SetUp() override {

    }

    void TearDown() override {

    }

    void createRandomData(size_t num)
    {
        pages.clear();
        num_pages = num;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, num_pages);
        pages.resize(num_pages);
        for(auto i =0 ; i < pages.size(); ++i) {
            pages[i] = distrib(gen);
        }
    }

    void createRandomData(size_t num, size_t mode)
    {
        pages.clear();
        num_pages = num;
        std::random_device rd;
        std::mt19937 gen(rd());
        if (mode == 0) {
            std::uniform_int_distribution<> distrib(1, num_pages);
            pages.resize(num_pages);
            for(auto i =0 ; i < pages.size(); ++i) {
                pages[i] = distrib(gen);
            }
        }
        else if (mode == 1) {
            std::normal_distribution<> distrib(num_pages/2, num_pages/3);
            auto random_int = [&distrib, &gen]{ return std::round(distrib(gen)); };
            pages.resize(num_pages);
            for(auto i =0 ; i < pages.size(); ++i) {
                pages[i] = random_int();
            }
        }
        else if (mode == 2) {
            std::chi_squared_distribution<> distrib(num_pages/3);
            pages.resize(num_pages);
            for(auto i =0 ; i < pages.size(); ++i) {
                pages[i] = distrib(gen);
            }
        }
        else if (mode == 3) {
            std::lognormal_distribution<> distrib(log(num_pages/2), log(num_pages/3));
            pages.resize(num_pages);
            for(auto i =0 ; i < pages.size(); ++i) {
                pages[i] = distrib(gen);
            }
        }

    }
    void setData(const std::vector<int>&& data) {
        pages = data;
    }
};

TEST_F(CacheTest, checkNullSizeException) {
    EXPECT_THROW({caches::ARC<int>(0);}, std::runtime_error);
}

TEST_F(CacheTest, consistCheck) {
    arc.setSize(20);
    size_t cnt_size = 10;
    for(auto size = 10, data_size = size*2; size < 100; size += 10, data_size = size*2) {
        for(auto i = 0; i < 1000; ++i) {
            for(auto mode = 0; mode < 4; ++mode) {
                createRandomData(data_size, mode);
                for(const auto& i : pages) {
                    arc.lookup_update(i, slow_get_page_int);
                }
            }
        }
    }
    for(auto size = 10, data_size = size*2; size < 100; size += 10, data_size = size*2) {
        for(auto i = 0; i < 1000; ++i) {
            for(auto mode = 0; mode < 4; ++mode) {
                createRandomData(data_size, mode);
                for(const auto& i : pages) {
                    ic.lookup_update(i, slow_get_page_int);
                }
            }
        }
    }
}

TEST_F(CacheTest, compare) {
    size_t n = 100;
    arc.setSize(n);
    ic.setSize(n);
    bool failed = false;
    for(auto i = 0; i < 100; ++i) {
        for(auto mode = 0; mode < 4; ++mode) {
            createRandomData(n*4, mode);
            ic.setBuffer(pages);
            for(const auto& i : pages) {
                arc.lookup_update(i, slow_get_page_int);
                ic.lookup_update(i, slow_get_page_int);
            }
            if (arc.getHits() > ic.getHits()) {
                failed = true;
                break;
            }
        }
    }
    EXPECT_FALSE(failed);
}


TEST_F(CacheTest, perfIdealTest) {
    createRandomData(1e3);
    ic.setSize(100);
    ic.setBuffer(pages);
    for(const auto& i : pages) {
        ic.lookup_update(i, slow_get_page_int);
    }
}

TEST_F(CacheTest, isEmptyInitiallyIdeal) {
    EXPECT_EQ(ic.getHits(), 0);
}

TEST_F(CacheTest, isEmptyInitiallyArc) {
    size_t cache_size = 4;
    EXPECT_EQ(arc.getHits(), 0);
}

TEST_F(CacheTest, perfArcTest) {
    createRandomData(1e6);
    arc.setSize(10);
    for(const auto& i : pages) {
        arc.lookup_update(i, slow_get_page_int);
    }
}

TEST_F(CacheTest, perfLargeArcTest) {
    createRandomData(1e6);
    arc.setSize(1e4);
    for(const auto& i : pages) {
        arc.lookup_update(i, slow_get_page_int);
    }
}

}
}
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
