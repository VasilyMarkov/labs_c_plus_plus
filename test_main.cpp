#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ideal_cache.h"
#include <fstream>
#include <vector>
#include "sstream"

std::vector<std::string> file_names;
std::string file_name = "test2.dat";

class IdealCacheTest : public ::testing::Test {
protected:
    caches::IdealCache<int> ic0;
};

TEST_F(IdealCacheTest, isEmptyInitially) {
    EXPECT_EQ(ic0.getHits(), 0);
}

TEST_F(IdealCacheTest, test1) {
    size_t expected_hits = 4;
    std::vector<int> pages;
    std::string line;
    std::ifstream infile("test2.dat");
    while(std::getline(infile, line)) {

    }
    for(const auto& i : pages) {
        ideal.lookup_update(i);
    }
    EXPECT_EQ(ic0.getHits(), expected_hits);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
