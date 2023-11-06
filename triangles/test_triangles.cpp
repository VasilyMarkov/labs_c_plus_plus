#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "triangles.hpp"

using namespace testing;

TEST(Test, TrianglesTest) {
    std::vector<int> triangles = {0,0,0, 3,0,0, 3,3,0, 1,0,0, 4,0,0, 4,3,0};
    std::vector<int> x_axis, y_axis, z_axis;

    std::multimap<int, size_t> x_points, y_points, z_points;

    prep::divideByAxis(triangles, x_axis, y_axis, z_axis);
    prep::sortingPoints(x_axis, y_axis, z_axis, x_points, y_points, z_points);

    std::vector<size_t> x_triangles, y_triangles, z_triangles;
    intersect::intersectByAxis(x_points, x_triangles);
    intersect::intersectByAxis(y_points, y_triangles);
    intersect::intersectByAxis(z_points, z_triangles);

    auto x_y_intersect{intersect::axisesCompare(x_triangles, y_triangles)};

//    EXPECT_THAT(intersect::axisesCompare(x_y_intersect, z_triangles), ElementsAre(1, 2));
    EXPECT_THAT(x_y_intersect, ElementsAre(1, 2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
