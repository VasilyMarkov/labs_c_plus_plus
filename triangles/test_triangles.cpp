#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <random>
#include "triangles.hpp"

using namespace testing;
enum class mode {
    nointersect,
    intersect
};

template <typename T>
void print(const std::vector<T>& vec) {
    for(auto const& i : vec) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

std::vector<int> generator(size_t n, mode mode) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> points;
    switch(mode) {
        case mode::nointersect:
            for(auto i = 0; i < n; ++i) {
                points.insert(std::end(points), {i, 0, 0,
                                                 0, i, 0,
                                                 0, 0, i});
            }
        break;
        case mode::intersect:
            points.insert(std::end(points), {1, 0, 0,
                                             0, 1, 0,
                                             0, 0, 1});
            for(auto i = 0; i < n-1; ++i) {
                points.insert(std::end(points), {2, 0, i,
                                                 0, 2, i,
                                                 0, 0, i});
            }
        break;
        default:
        break;
    }

    return points;
}

TEST(Test, TrianglesTest) {
    size_t n = 3;

//    std::vector<int> triangles = {2,0,0, 0,2,0, 0,0,2, 3,0,0, 0,3,0, 0,0,3};
    std::vector<int> triangles{generator(n, mode::intersect)};
//    print(generator(n, mode::intersect));
    std::vector<int> x_axis, y_axis, z_axis;

    std::multimap<int, size_t> x_points, y_points, z_points;

    prep::divideByAxis(triangles, x_axis, y_axis, z_axis);
    prep::sortingPoints(x_axis, y_axis, z_axis, x_points, y_points, z_points);

    std::vector<size_t> x_triangles, y_triangles, z_triangles;
    intersect::intersectByAxis(x_points, x_triangles);
    intersect::intersectByAxis(y_points, y_triangles);
    intersect::intersectByAxis(z_points, z_triangles);

    auto x_y_intersect{intersect::axisesCompare(x_triangles, y_triangles)};
    auto intersect{intersect::axisesCompare(x_y_intersect, z_triangles)};
    print(intersect);
    EXPECT_THAT(intersect, ElementsAre(1, 2, 3));
//    EXPECT_THAT(x_y_intersect, ElementsAre(1, 2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
