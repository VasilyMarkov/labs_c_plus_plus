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
template <typename T>
class Generator {
private:
    std::vector<T> points;
    size_t size{0};
public:
    Generator(size_t n): size(n) {
        intersect();
    }

    std::vector<T> getPoints() const {return points;}

    void intersect() {
        points.insert(std::end(points), {1, 0, 0,
                                         0, 1, 0,
                                         0, 0, 1});
        for(auto i = 0; i < size-1; ++i) {
            points.insert(std::end(points), {2, 0, i,
                                             0, 2, i,
                                             0, 0, i});
        }
    }
    void intersect2D() {
        for(auto i = 1; i < size+1; ++i) {
            points.insert(std::end(points), {i, 0, 0,
                                             0, i, 0,
                                             0, 0, 0});
        }
    }
    void noIntersect() {
        for(auto i = 1; i < size; ++i) {
            points.insert(std::end(points), {i, 0, 0,
                                             0, i, 0,
                                             0, 0, i});
        }
    }
    void random(distr_t distr) {
        switch(distr) {
            case distr_t::uniform: {
                std::random_device rd;
                std::mt19937 gen(rd());
                for(auto i = 0; i < size; ++i) {
                    std::uniform_int_distribution<> distrib(-size/2, size/2);
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
                for(auto i = 0; i < size; ++i) {
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
 
size_t n = 1e3;
Generator<int> generator(n);

TEST(Test, noValidTriangles) {
    std::vector<int> points = {0,0,0, 0,0,0, 0,0,0,
                               1,0,0, 0,0,0, 0,0,0,
                               1,0,0, 2,0,0, 3,0,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle2D) {
    std::vector<int> points = {0,2,0, 1,2,0, 0,3,0,
                               0,0,0, 1,0,0, 0,1,0};
    auto result = intersectTriangles(createTriangles(points));
    EXPECT_EQ(result, std::nullopt);
}

TEST(Test, noIntersectTriangle2D_1) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               0,2,0, 1,2,0, 0,3,0};

    auto result = intersectTriangles(createTriangles(points));
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

TEST(Test, intersectTriangle2D_3) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               -1,-1,0, -1,4,0, 4,-1,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle2D_4) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               0,0,0, -1,0,0, 0,-1,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle2D_5) {
    std::vector<int> points = {0,0,0, 1,0,0, 0,1,0,
                               0,0,0, -1,0,0, 0,1,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_1) {
    std::vector<int> points = {2,0,0, 0,2,0, 0,0,2,
                               3,0,0, 0,3,0, 0,0,1};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_2) {
    std::vector<int> points = {1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,5, 2,0,5};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_3) {
    std::vector<int> points = {0,0,0, 2,0,0, 0,2,0,
                               1,1,0, 0,0,3, 1,0,3};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_4) {
    std::vector<int> points = {0,0,0, 2,0,0, 0,2,0,
                               0,1,0, 0,0,3, 1,0,3};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_5) {
    std::vector<int> points = {0,0,0, 2,0,0, 0,2,0,
                               0,0,0, 2,0,0, 1,0,3};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectTriangle3D_6) {
    std::vector<int> points = {1,1,0, 3,1,0, 1,3,0,
                               1,1,0, 1,2,3, 5,4,8};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectSameTriangle3D) {
    std::vector<int> points = {1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,0, 0,0,1};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1));
}

TEST(Test, intersectFiveTriangles3D) {
    std::vector<int> points = {1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,0, 0,0,1,
                               1,0,0, 0,1,0, 0,0,1};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result.value(), ElementsAre(0, 1, 2, 3, 4));
}

TEST(Test, intersectClosedTriangles1) {
    double eps = 1e-7;
    std::vector<double> points = {0,0,0, 1,0,0, 0,1,0,
                                  1+eps,0,0, 0,1,0, 1+eps,1,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result, std::nullopt);
}

TEST(Test, intersectClosedTriangles2) {
    double eps = 1e-7;
    std::vector<double> points = {0,0,0, 1,0,0, 0,1,0,
                                  -eps,0,0, -1,0,0, -eps,1,0};

    auto result = intersectTriangles(createTriangles(points));
    EXPECT_THAT(result, std::nullopt);
}

TEST(Test, TrianglesTest) {
    auto points = generator.getPoints();
    auto result = intersectTriangles(createTriangles(points));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
