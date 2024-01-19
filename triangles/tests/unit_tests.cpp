#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <optional>
#include "linal.hpp"
#include <iostream>
using namespace testing;

template<typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& vec) {
    for(const auto& i : vec) {
        out << i << ' ';
    }
    return out;
}

TEST(UnitTest, isPlane) {
    double eps = 1e-7;
    std::vector<double> points = {0,0,0, 1,0,0, 0,1,0,
                                  1,0,0, 0,1,0, 0,0,1,
                                  0,0,0, eps,0,0, 0,1,0,
                                  0,0,0, eps,0,0, 0,eps,0,
                                  eps,0,0, 0,eps,0, 0,0,eps,
                                  0,0,0, eps,0,0, 0,eps,0,
                                  1+eps,0,0, 2-eps,0,0, 3+eps,0,0};

    const auto triangles = createTriangles(points);
    auto result = true;
    std::vector<int> indexes;

    for(auto i = 0; i < triangles.size(); ++i) {
        if(!triangles.at(i).isPlane()) {
            result = false;
            indexes.push_back(i);
        }
    }
    if(!result)
        std::cout << "--- Incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);  
}

TEST(UnitTest, isNotPlane) {
    double eps = 1e-7;
    std::vector<double> points = {0,0,0, 0,0,0, 0,0,0,
                                  1,0,0, 0,0,0, 0,0,0,
                                  1,0,0, 2,0,0, 3,0,0,
                                  1+eps,0,0, 2+eps,0,0, 3+eps,0,0};
    const auto triangles = createTriangles(points);
    auto result = false;
    std::vector<int> indexes;

    for(auto i = 0; i < triangles.size(); ++i) {
        if(triangles.at(i).isPlane()) {
            result = true;
            indexes.push_back(i);
        }
    }
    if(result)
        std::cout << "--- Incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_FALSE(result);
}

TEST(UnitTest, triangleProjection) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,  //z = 0
                               0,0,0, 1,0,0, 0,0,1,  //y = 0
                               0,0,0, 0,1,0, 0,0,1,  //x = 0
                               1,0,0, 0,1,0, 1,0,1,  //x+y = 1
                               1,0,0, 1,1,0, 0,0,1,  //x+z = 1
                               1,1,0, 0,1,0, 0,0,1,  //y+z = 1
                               1,0,0, 0,1,0, 0,0,1}; //x+y+z = 1
    auto triangles = createTriangles(points);
 
    std::vector<Triangle2d> proj = {{Point2d(0,0),Point2d(1,0), Point2d(0,1)},
                                    {Point2d(0,0),Point2d(1,0), Point2d(0,1)},
                                    {Point2d(0,0),Point2d(1,0), Point2d(0,1)},
                                    {Point2d(1,0),Point2d(0,0), Point2d(1,1)},
                                    {Point2d(1,0),Point2d(1,1), Point2d(0,0)},
                                    {Point2d(1,1),Point2d(0,1), Point2d(0,0)},
                                    {Point2d(1,0),Point2d(0,1), Point2d(0,0)}};

    auto result = true;
    std::vector<int> indexes;
    for(auto i = 0; i < triangles.size(); ++i) {
        if(!(triangles.at(i).triangleProjection() == proj.at(i))) {
            result = false;
            indexes.push_back(i);
        }        
    }

    if(!result)
        std::cout << "--- Incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
