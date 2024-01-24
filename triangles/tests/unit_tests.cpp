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

TEST(UnitTest, planeConstructor) {
    const std::vector<Point3d> points = {
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };
    Plane plane (points[0], points[1], points[2]);
    EXPECT_EQ(plane.a, 1);
    EXPECT_EQ(plane.b, 1);
    EXPECT_EQ(plane.c, 1);
    EXPECT_EQ(plane.d, -1);
}

TEST(UnitTest, isPlane) {

    const std::vector<double> points = 
    {
        0,0,0, 1,0,0, 0,1,0,
        1,0,0, 0,1,0, 0,0,1,
        0,0,0, eps,0,0, 0,1,0,
        0,0,0, eps,0,0, 0,eps,0,
        eps,0,0, 0,eps,0, 0,0,eps,
        0,0,0, eps,0,0, 0,eps,0,
        // 1+eps,0,0, 2-eps,0,0, 3+eps,0,0
    };

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
        std::cout << "--- IsPlane Incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);  
}

TEST(UnitTest, isNotPlane) {
    const  std::vector<double> points = 
    {
        0,0,0, 0,0,0, 0,0,0,
        1,0,0, 0,0,0, 0,0,0,
        1,0,0, 2,0,0, 3,0,0,
        1+eps,0,0, 2+eps,0,0, 3+eps,0,0
    };
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
        std::cout << "--- IsNotPlane incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_FALSE(result);
}

TEST(UnitTest, triangleProjection) {
    const std::vector<double> points = 
    {
        0,0,0, 1,0,0, 0,1,0,  //z = 0
        0,0,0, 1,0,0, 0,0,1,  //y = 0
        0,0,0, 0,1,0, 0,0,1,  //x = 0
        1,0,0, 0,1,0, 1,0,1,  //x+y = 1
        1,0,0, 1,1,0, 0,0,1,  //x+z = 1
        1,1,0, 0,1,0, 0,0,1,  //y+z = 1
        1,0,0, 0,1,0, 0,0,1   //x+y+z = 1
    }; 

    const auto triangles = createTriangles(points);

    const std::vector<Triangle2d> proj = 
    {
        {Point2d(0,0),Point2d(1,0), Point2d(0,1)},
        {Point2d(0,0),Point2d(1,0), Point2d(0,1)},
        {Point2d(0,0),Point2d(1,0), Point2d(0,1)},
        {Point2d(1,0),Point2d(0,0), Point2d(1,1)},
        {Point2d(1,0),Point2d(1,1), Point2d(0,0)},
        {Point2d(1,1),Point2d(0,1), Point2d(0,0)},
        {Point2d(1,0),Point2d(0,1), Point2d(0,0)}
    };

    auto result = true;
    std::vector<int> indexes;
    for(auto i = 0; i < triangles.size(); ++i) {
        Triangle2d triangle_proj{triangles.at(i).projection(triangles.at(i).vertices)};
        if(!(triangle_proj == proj.at(i))) {
            result = false;
            indexes.push_back(i);
        }        
    }

    if(!result)
        std::cout << "--- TriangleProjection incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}


TEST(UnitTest, relativePosition) {
    const std::vector<std::vector<double>> dets = 
    {
        {0,0,0},
        {0,2,0},
        {0,2,3},
        {0,-1,0},
        {0,-2,-1},
        {0,2,-1},
        {2,3,4},
        {-1,-4,-2},
        {eps,-eps, eps}
    };
    const std::unordered_map<size_t, position> positions = 
    {
        {0, position::same_plane},
        {1, position::diff_half_space},
        {2, position::diff_half_space},
        {3, position::diff_half_space},
        {4, position::diff_half_space},
        {5, position::diff_half_space},
        {6, position::same_half_space},
        {7, position::same_half_space},
        {8, position::diff_half_space},
    };
    auto result = true;
    std::vector<int> indexes;
    for(size_t i = 0; i < dets.size(); ++i) {
        if(checkRelativePosition(dets.at(i)) != positions.at(i)) {
            result = false;
            indexes.push_back(i);
        }

    }
    if(!result)
        std::cout << "--- RelativePosition incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);  
}

TEST(UnitTest, separablePlane) {
    const std::vector<double> points = 
    {
        3,0,0, 0,3,0, 0,0,3,              //base triangle
        3,0,0, 0,3,6, 0,0,6,              //intersection between 2 points
        3,0,0, 0,3,0, 3,3,3,              //intersection between 2 lines
        1.5,1.5,0, 3,3,0, 3,3,3,          //intersection between line and point
        1,1,1, 3,3,0, 3,3,3,              //intersection between plane and point
        1.5,1.5,0, 0,1.5,1.5, 3,3,3      //intersection between plane and line
    };
    const auto triangles = createTriangles(points);
    auto result = true;
    std::vector<int> indexes;

    for(auto i = 1; i < triangles.size(); ++i) {
        if(!triangles.at(0).separable_plane_from(triangles.at(i))) {
            result = false;
            indexes.push_back(i);
        }
    }
    if(!result)
        std::cout << "--- SeparablePlane incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}

TEST(UnitTest, noSeparablePlane) {
    std::vector<double> points = {
        0,0,0, 0,1,0, 0,0,1,
        // 0,eps,0, 0,1,eps, eps,0,1,
        0,0,2, 1,0,2, 1,0,3
    };
    const auto triangles = createTriangles(points);
    auto result = true;
    std::vector<int> indexes;

    for(auto i = 1; i < triangles.size(); ++i) {
        if(triangles.at(0).separable_plane_from(triangles.at(i))) {
            result = false;
            indexes.push_back(i);
        }
    }
    if(!result)
        std::cout << "--- noSeparablePlane incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}

TEST(UnitTest, separableLine) {
    const std::vector<double> points = 
    {
        3,0,0, 0,3,0, 0,0,0,              //base triangle
        3,0,0, 4,0,0, 4,3,0,              //intersection between 2 points
        3,0,0, 0,3,0, 3,3,0,              //intersection between 2 lines
        1.5,1.5,0, 2,3,0, 3,2,0,          //intersection between line and point
        3,0,0, 0,3,0, 0,0,0,              //intersection between plane and point
        3+eps,0,0, 0,3+eps,0, 0,0,0,      //intersection between closed triangles
    };
    const auto triangles = createTriangles(points);
    auto result = true;
    std::vector<int> indexes;

    for(auto i = 1; i < triangles.size(); ++i) {
        if(!triangles.at(0).separable_line_from(triangles.at(i))) {
            result = false;
            indexes.push_back(i);
        }
    }
    if(!result)
        std::cout << "--- SeparableLine incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}

TEST(UnitTest, noSeparableLine) {
    std::vector<double> points = 
    {
        0,2,0, 1,2,0, 0,3,0,
        0,0,0, 1,0,0, 0,1,0
    };
    std::vector<double> points1 = 
    {
        0,0,0, 0,2,0, 0,0,2,
        1,0,0, 1,2,0, 3,0,0
    };
    const auto triangles = createTriangles(points1);

    auto result = true;
    std::vector<int> indexes;
    for(auto i = 1; i < triangles.size(); ++i) {
        if(triangles.at(0).separable_line_from(triangles.at(i))) {
            result = false;
            indexes.push_back(i);
        }
    }
    if(!result)
        std::cout << "--- NoSeparableLine incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}

TEST(UnitTest, pointInsideTriangle) {

    Triangle2d triangle = {{0,0}, {4,0}, {2,2}};
    std::vector<Point2d> pt = 
    {
        {0,0},
        {1,1},
        {2,0},
        {eps,0},
    };

    auto result = true;
    std::vector<int> indexes;
    for(auto i = 0; i < pt.size(); ++i) {
        if(!triangle.pointInTriangle(pt.at(i))) {
            result = false;
            indexes.push_back(i);
        }
    }
    if(!result)
        std::cout << "--- PointInsideTriangle incorrect indexes: " << indexes << "---" << std::endl;
    EXPECT_TRUE(result);
}

TEST(UnitTest, pointIsNotInTriangle) {

    std::vector<double> points = {
        0,0,0, 0,1,0, 0,0,1,
        // 0,eps,0, 0,1,eps, eps,0,1,
        0,0,2, 1,0,2, 1,0,3
    };
    const auto triangles = createTriangles(points);
    auto result = false;
    std::vector<double> signs_tr1, signs_tr2;
    auto thisT = triangles.at(0);
    auto another = triangles.at(1);
    for (size_t i = 0; i < another.vertices.size(); ++i) {  
        auto det_this = thisT.det4(thisT.vertices[0], thisT.vertices[1], thisT.vertices[2], another.vertices[i]);
        auto det_another = another.det4(another.vertices[0], another.vertices[1], another.vertices[2], thisT.vertices[i]);
        if(det_this == 0) {
            Triangle2d proj(thisT.projection(thisT.vertices));
            auto point = thisT.projection(std::array<Point3d, 1>{another.vertices[i]});
            if(!proj.pointInTriangle(point[0])) result = true; 
        }
    }
    
    EXPECT_TRUE(result);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
