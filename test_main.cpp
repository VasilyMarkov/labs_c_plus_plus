#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ideal_cache.h"
#include "arc.h"
#include <vector>

class IdealCacheTest : public ::testing::Test {
protected:
    caches::IdealCache<int> ic0;
};

class ArcTest : public ::testing::Test {
protected:
    caches::ARC<int>* arc0;
    caches::ARC<int>* arc1;
};

TEST_F(IdealCacheTest, isEmptyInitially) {
    EXPECT_EQ(ic0.getHits(), 0);
}

TEST_F(ArcTest, isEmptyInitially) {
    size_t cache_size = 4;
    arc0 = new caches::ARC<int>(cache_size);
    EXPECT_EQ(arc0->getHits(), 0);
    delete arc0;
}

TEST_F(IdealCacheTest, test1) {
    std::vector<int> pages = {1, 2, 1, 2, 1, 2};
    size_t expected_hits = 4;
    for(const auto& i : pages) {
        ic0.lookup_update(i);
    }
    EXPECT_EQ(ic0.getHits(), expected_hits);
}

TEST_F(IdealCacheTest, test2) {
    std::vector<int> pages = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    size_t expected_hits = 7;
    for(const auto& i : pages) {
        ic0.lookup_update(i);
    }
    EXPECT_EQ(ic0.getHits(), expected_hits);
}

TEST_F(ArcTest, test1) {
    std::vector<int> pages = {1, 2, 1, 2, 1, 2};
    size_t cache_size = 2;
    size_t expected_hits = 4;

    arc0 = new caches::ARC<int>(cache_size);
    for(const auto& i : pages) {
        arc0->lookup_update(i);
    }
    EXPECT_EQ(arc0->getHits(), expected_hits);
    delete arc0;
}

TEST_F(ArcTest, test2) {
    std::vector<int> pages = {1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};
    size_t cache_size = 5;
    size_t expected_hits = 7;

    arc1 = new caches::ARC<int>(cache_size);
    for(const auto& i : pages) {
        arc1->lookup_update(i);
    }
    EXPECT_EQ(arc1->getHits(), expected_hits);
    delete arc1;
}

TEST_F(ArcTest, test3) {
    std::vector<int> pages = {1, 2, 3, 4, 5, 6, 7, 8};
    size_t cache_size = 4;
    size_t expected_hits = 0;

    arc1 = new caches::ARC<int>(cache_size);
    for(const auto& i : pages) {
        arc1->lookup_update(i);
    }
    EXPECT_EQ(arc1->getHits(), expected_hits);
    delete arc1;
}

TEST_F(ArcTest, test4) {
    std::vector<int> pages = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
    size_t cache_size = 4;
    size_t expected_hits = 8;

    arc1 = new caches::ARC<int>(cache_size);
    for(const auto& i : pages) {
        arc1->lookup_update(i);
    }
    EXPECT_EQ(arc1->getHits(), expected_hits);
    delete arc1;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
