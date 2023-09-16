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
        num_pages = num;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, num_pages);
        pages.resize(num_pages);
        for(auto i =0 ; i < pages.size(); ++i) {
            pages[i] = distrib(gen);
        }
    }
    void setData(const std::vector<int>&& data) {
        pages = data;
    }
};

TEST_F(CacheTest, isEmptyInitiallyIdeal) {
    EXPECT_EQ(ic.getHits(), 0);
}

TEST_F(CacheTest, isEmptyInitiallyArc) {
    size_t cache_size = 4;
    EXPECT_EQ(arc.getHits(), 0);
}

TEST_F(CacheTest, test1) {
    std::vector<int> pages = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    setData(std::move(pages));
    size_t expected_hits = 7;
    arc.setSize(4);
    for(const auto& i : pages) {
        arc.lookup_update(i);
    }
    EXPECT_EQ(arc.getHits(), expected_hits);
}

//TEST_F(CacheTest, compareHitsTest) {
//    createRandomData(1e2);
//    arc.setSize(10);
//    for(const auto& i : pages) {
//        arc.lookup_update(i);
//        ic.lookup_update(i);
//    }
//    EXPECT_EQ(arc.getHits(), ic.getHits());
//}

TEST_F(CacheTest, perfIdealTest) {
    createRandomData(1e6);
    for(const auto& i : pages) {
        ic.lookup_update(i);
    }
}

TEST_F(CacheTest, perfArcTest) {
    createRandomData(1e6);
    arc.setSize(10);
    for(const auto& i : pages) {
        arc.lookup_update(i);
    }
}

TEST_F(CacheTest, perfLargeArcTest) {
    createRandomData(1e6);
    arc.setSize(1e4);
    for(const auto& i : pages) {
        arc.lookup_update(i);
    }
}

}
}
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
