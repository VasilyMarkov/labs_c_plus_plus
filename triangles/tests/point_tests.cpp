#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "linal.hpp"
#include <iostream>

using namespace testing;

TEST(UnitTest, point2dConstructors) {
    Point2d point1{1.0,1.0};
    EXPECT_DOUBLE_EQ(point1.x(), 1.0);
    EXPECT_DOUBLE_EQ(point1.y(), 1.0);
    
    Point2d point2(point1);
    EXPECT_DOUBLE_EQ(point2.x(), 1.0);
    EXPECT_DOUBLE_EQ(point2.y(), 1.0);

    Point2d point3(Point3d{1.0, 1.0, 1.0});
    EXPECT_DOUBLE_EQ(point3.x(), 1.0);
    EXPECT_DOUBLE_EQ(point3.y(), 1.0);
}

TEST(UnitTest, point3dConstructors) {
    Point3d point1{1.0,1.0,1.0};
    EXPECT_DOUBLE_EQ(point1.x(), 1.0);
    EXPECT_DOUBLE_EQ(point1.y(), 1.0);
    EXPECT_DOUBLE_EQ(point1.z(), 1.0);
    
    Point3d point2(point1);
    EXPECT_DOUBLE_EQ(point2.x(), 1.0);
    EXPECT_DOUBLE_EQ(point2.y(), 1.0);
    EXPECT_DOUBLE_EQ(point2.z(), 1.0);
    
    Point3d point3(Point2d{1.0, 1.0});
    EXPECT_DOUBLE_EQ(point3.x(), 1.0);
    EXPECT_DOUBLE_EQ(point3.y(), 1.0);
    EXPECT_TRUE(point3.z() != point3.z());
}

TEST(UnitTest, point2dEqualOperator) {
    Point2d point1{1.0,1.0};
    Point2d point2{1.0,1.0};
    EXPECT_TRUE(point1 == point2);

    Point2d point3{1.0+eps,1.0};
    Point2d point4{1.0,1.0};
    EXPECT_FALSE(point3 == point4);
}

TEST(UnitTest, point3dEqualOperator) {
    Point3d point1{1.0,1.0,1.0};
    Point3d point2{1.0,1.0,1.0};
    EXPECT_TRUE(point1 == point2);

    Point3d point3{1.0,1.0+eps,1.0};
    Point3d point4{1.0,1.0,1.0};
    EXPECT_FALSE(point3 == point4);
}

TEST(UnitTest, point2dMinusOperator) {
    Point2d point1{3.0,2.0};
    Point2d point2{1.0,1.0};
    Point3d point3{1.0,1.0,1.0};

    auto result = point1 - point2;
    EXPECT_DOUBLE_EQ(result.x(), 2.0);
    EXPECT_DOUBLE_EQ(result.y(), 1.0);

    result = point2 - point1;
    EXPECT_DOUBLE_EQ(result.x(), -2.0);
    EXPECT_DOUBLE_EQ(result.y(), -1.0);

    result = point1 - point3;
    EXPECT_DOUBLE_EQ(result.x(), 2.0);
    EXPECT_DOUBLE_EQ(result.y(), 1.0);

    result = point3 - point1;
    EXPECT_DOUBLE_EQ(result.x(), -2.0);
    EXPECT_DOUBLE_EQ(result.y(), -1.0);
}