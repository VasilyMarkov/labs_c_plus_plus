#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <random>
#include <optional>
#include "linal.hpp"

using namespace testing;
enum class mode {
    nointersect,
    intersect,
    random
};

template <typename T>
void print(const std::vector<T> vec) {
    for(auto const& i : vec) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

enum class distr_t {
    uniform,
    normal,
    bernulli
};

class Generator {
private:

public:

    void intersect(std::vector<int>& points, size_t n) const {
        points.insert(std::end(points), {1, 0, 0,
                                         0, 1, 0,
                                         0, 0, 1});
        for(auto i = 0; i < n-1; ++i) {
            points.insert(std::end(points), {2, 0, i,
                                             0, 2, i,
                                             0, 0, i});
        }
    }

    void noIntersect(std::vector<int>& points, size_t n) const {
        for(auto i = 1; i < n; ++i) {
            points.insert(std::end(points), {i, 0, 0,
                                             0, i, 0,
                                             0, 0, i});
        }
    }
    template <typename T>
    void random(std::vector<T>& points, size_t n, distr_t distr) const {
        switch(distr) {
            case distr_t::uniform: {
                std::random_device rd;
                std::mt19937 gen(rd());
                for(auto i = 0; i < n; ++i) {
                    std::uniform_int_distribution<> distrib(-n/2, n/2);
                    points.insert(std::end(points), {
                        static_cast<double>(distrib(gen)), static_cast<double>(distrib(gen)), static_cast<double>(distrib(gen)),
                        static_cast<double>(distrib(gen)), static_cast<double>(distrib(gen)), static_cast<double>(distrib(gen)),
                        static_cast<double>(distrib(gen)), static_cast<double>(distrib(gen)), static_cast<double>(distrib(gen))}
                    );
                }
            }
            break;
            case distr_t::normal: {
                std::random_device rd;
                std::mt19937 gen(rd());
                for(auto i = 0; i < n; ++i) {
                    std::normal_distribution<> distrib(0, 10);
                    points.insert(std::end(points), {
                        distrib(gen), distrib(gen), distrib(gen),
                        distrib(gen), distrib(gen), distrib(gen),
                        distrib(gen), distrib(gen), distrib(gen)}
                    );
                }
            }
            break;
            case distr_t::bernulli:
            break;
            default:
            break;
        }
    }
};

TEST(Test, noValidTriangle_1) {
    std::vector<int> points = {0,0,0, 0,0,0, 0,0,0,
                               0,0,0, 0,0,0, 0,0,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noValidTriangle_2) {
    std::vector<int> points = {0,0,0, 0,1,0, 0,0,0,
                               0,0,0, 0,2,0, 0,0,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle2D) {
    std::vector<int> points = {0,2,0, 1,2,0, 0,3,0,
                               0,0,0, 1,0,0, 0,1,0};
    auto result = intersectTriangles(createTriangles(points));
    auto result1 = createTriangles(points);
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle2D_1) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               0,2,0, 1,2,0, 0,3,0};

    auto result = intersectTriangles(createTriangles(points));
    auto result1 = createTriangles(points);
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle2D_2) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               2,0,0, 0,2,0, 2,2,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle2D_3) {
    std::vector<int> points = {0,0,0, 0,1,0, 0,0,1,
                               0,0,2, 0,3,0, 0,0,3};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle3D_1) {
    std::vector<int> points = {0,0,0, 0,1,0, 0,0,1,
                               0,0,2, 1,0,2, 1,0,3};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle3D_2) {
    std::vector<int> points = {1,0,0, 0,1,0, 0,0,1,
                               2,0,0, 0,2,0, 0,0,2};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, intersectTriangle2D_1) {
    std::vector<int> points = {0,0,0, 0,1,0, 1,0,0,
                               0,0,0, 0,2,0, 2,0,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle2D_2) {
    std::vector<int> points = {0,0,0, 0,1,0, 1,0,0,
                               0,0,0, 0,-1,0, -1,0,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectSameTriangle2D) {
    std::vector<int> points = {0,0,0, 0,1,0, 1,0,0,
                               0,0,0, 0,1,0, 1,0,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle2D) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               -1,-1,0, -1,4,0, 4,-1,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_1) {
    std::vector<int> points = {2,0,0, 0,2,0, 0,0,2,
                               3,0,0, 0,3,0, 0,0,1};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectSameTriangle3D) {
    std::vector<int> points = {1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,0, 0,0,1};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, TrianglesTest) {
    size_t n = 1e2;

    std::vector<int> input = {2,0,0, 0,2,0, 0,0,2,
                              3,0,0, 0,3,0, 0,0,1,
                              3,0,0, 0,3,0, 0,0,3};

    std::vector<double> points;
    points.reserve(n);
    Generator generator;
    generator.random(points, n, distr_t::uniform);
    auto result = intersectTriangles(createTriangles(points));
    if(result) {
//        print(const_cast<std::vector<size_t>&>(result.value()));
        std::cout << result.value().size() << std::endl;
    }
    else {
        std::cout << "No intersection" << std::endl;
    }
//    EXPECT_THAT(x_y_intersect, ElementsAre(1, 2));
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
