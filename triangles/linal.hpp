#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <assert.h>
#include <array>
#include <optional>

const float fit_tolerance  = 0.00001;
const float inter_area_width = 100.0;

enum area_t { RIGHT_SIDE, INTER_SIDE, LEFT_SIDE };

struct Point2d {
public:
    float x = NAN;
    float y = NAN;
public:
    Point2d() = default;
    Point2d(float x_i, float y_i): x(x_i), y(y_i){}
    Point2d(const Point2d& other): x(other.x), y(other.y){}
};

struct Point3d: public Point2d {
    float z = NAN;
    Point3d() = default;
    Point3d(float x_i, float y_i, float z_i): Point2d(x_i, y_i), z(z_i){}
    Point3d(const Point3d& other): Point2d(other.x, other.y), z(other.z){}
    Point3d& operator=(const Point3d& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    bool valid() const { return !(x != x || y != y); }

    void print() const {
        std::cout << '(' << x << ", " << y << ", " << ')';
    }
    bool equal(const Point3d& rhs) const {
        return (std::abs(x-rhs.x) < fit_tolerance) &&
               (std::abs(y-rhs.y) < fit_tolerance) &&
               (std::abs(z-rhs.z) < fit_tolerance);
    }
};

struct Line {
    float a = -1.0f, b = 1.0f, c = 0.0f;

    Line(const Point2d &p1, const Point2d &p2) {
        float angle = std::atan((p2.y - p1.y) / (p2.x - p1.x));
        float sin_angle = std::sin(angle);
        float cos_angle = std::sqrt(1.0 - sin_angle * sin_angle);
        Point2d normal_vect{-sin_angle, cos_angle};
        a = normal_vect.x;
        b = normal_vect.y;
        c = -(p1.x * normal_vect.x + p1.y * normal_vect.y);
        if(b == 0 && c == 0) a = abs(a);
        if(a == 0 && c == 0) b = abs(b);
        if(a == 0 && b == 0) c = abs(c);
    }

    enum area_t get_side_area(const Point2d& point) const {
    float side_offset = a * point.x + b * point.y + c;
        if (side_offset > 0.0 + fit_tolerance * inter_area_width)
          return LEFT_SIDE;

        if (side_offset > 0.0 - fit_tolerance * inter_area_width)
          return INTER_SIDE;

        return RIGHT_SIDE;
    }

    bool valid() const { return !(a != a || b != b || c != c); }
};

struct Plane {
    float a = NAN, b = NAN, c = NAN, d = NAN;
    Plane() = default;
    Plane(const Point3d& p1, const Point3d& p2, const Point3d& p3) {
        auto a1 = p2.x - p1.x;
        auto b1 = p2.y - p1.y;
        auto c1 = p2.z - p1.z;
        auto a2 = p3.x - p1.x;
        auto b2 = p3.y - p1.y;
        auto c2 = p3.z - p1.z;

        a = b1 * c2 - b2 * c1;
        b = a2 * c1 - a1 * c2;
        c = a1 * b2 - b1 * a2;
        d = (- a * p1.x - b * p1.y - c * p1.z);
    }

    enum area_t get_side_area(const Point3d& point) const {
        float side_offset = a * point.x + b * point.y + c * point.z + d;
        auto treshold = fit_tolerance * inter_area_width;
        if (side_offset > 0.0 + treshold)
          return RIGHT_SIDE;

        if (side_offset > 0.0 - treshold)
          return INTER_SIDE;

        return LEFT_SIDE;
    }
};


struct Triangle2d {
    std::array<Point2d, 3> vertices;
    Triangle2d() = default;
    Triangle2d(const Point2d& p1, const Point2d& p2, const Point2d& p3) {
        vertices[0] = p1, vertices[1] = p2, vertices[2] = p3;
    }
    bool operator==(const Triangle2d other) {
        bool isEqual = true;
        for(auto i = 0; i < vertices.size(); ++i) {
            isEqual = vertices.at(i).x == other.vertices.at(i).x &&
                      vertices.at(i).y == other.vertices.at(i).y;
            if(!isEqual) return isEqual;
        }
        return isEqual;
    }

    Line get_side(int index) const {
        assert(!vertices.empty());
        auto vsz = vertices.size();
        if (index < 0)
        index += (1 + (index / vsz)) * vsz;
        auto tmp1 = index % vsz;
        auto tmp2 = (index + 1) % vsz;
        Line ret(vertices[index % vsz], vertices[(index + 1) % vsz]);
        return ret;
    }
};

struct Triangle3d {
private:
    Plane plane;
    std::array<Point3d, 3> vertices;
public:
    Triangle3d() = default;
    Triangle3d(const Point3d& p1, const Point3d& p2, const Point3d& p3): plane(p1, p2, p3) {
        vertices[0] = p1, vertices[1] = p2, vertices[2] = p3;

    }
    Triangle3d& operator=(const Triangle3d& other) {
        plane.a = other.plane.a;
        plane.b = other.plane.b;
        plane.c = other.plane.c;
        vertices = other.vertices;
        return *this;
    }

    bool operator==(const Triangle3d other) {
        bool isEqual = true;
        for(auto i = 0; i < vertices.size(); ++i) {
            isEqual = vertices.at(i).x == other.vertices.at(i).x &&
                      vertices.at(i).y == other.vertices.at(i).y &&
                      vertices.at(i).z == other.vertices.at(i).z;
            if(!isEqual) return isEqual;
        }
        return isEqual;
    }

    Point3d crossProduct(const Point3d& p1, const Point3d& p2) const {
        Point3d cross(p1.y*p2.z-p1.z*p1.y,
                      p1.z*p2.x-p1.x*p1.z,
                      p1.x*p2.y-p1.y*p1.x);
        return cross;
    }

    bool isParallel(const Triangle3d& another) const {

        auto cross = crossProduct(Point3d(plane.a, plane.b, plane.c),
                                  Point3d(another.plane.a, another.plane.b, another.plane.c));
        if(cross.x == 0 && cross.y == 0 && cross.z == 0 ) return true;

        return false;
    }

    bool isSamePlane(const Triangle3d& another) const{
        auto eps = fit_tolerance*inter_area_width;
        auto dist = std::abs(plane.d-another.plane.d)/std::hypot(plane.a, plane.b, plane.c);
        if(std::abs(dist) < eps) {
            return true;
        }
        return false;
    }

    bool valid() const {
        if (vertices.size() == 0)
          return false;

        for (size_t i = 0; i < vertices.size(); i++)
          if (!vertices[i].valid())
            return false;

        return true;
    }

    bool separable_line_from(const Triangle3d& another) const {
        auto anotherProjection = another.triangleProjection();
        auto thisProjection = triangleProjection();
        for (size_t i = 0; i < thisProjection.vertices.size(); i++) {
          Line div_line = thisProjection.get_side(i);

        enum area_t half_space = div_line.get_side_area(thisProjection.vertices[(i + 2) % thisProjection.vertices.size()]);

        if (half_space == INTER_SIDE)
            continue;

        bool flag = true;
        for (size_t j = 0; j < anotherProjection.vertices.size(); j++) {
            Point2d vertex = anotherProjection.vertices[j];
            if (half_space == div_line.get_side_area(vertex)) {
                flag = false;
                break;
            }
        }
        if (flag)
            return true;
        }
        return false;
    }

    bool intersect(const Triangle3d& another) const {
        auto one = separable_line_from(another);
        auto second = another.separable_line_from(*this);
        auto result = !(one || second);
        return result;
    }

    Triangle2d triangleProjection() const {
        assert(plane.a != 0 || plane.b != 0 || plane.c != 0);
        Point2d p1, p2, p3;
        if(plane.b == 0 && plane.c == 0) { //projection to YZ
            p1.x = vertices[0].y, p1.y = vertices[0].z;
            p2.x = vertices[1].y, p2.y = vertices[1].z;
            p3.x = vertices[2].y, p3.y = vertices[2].z;
        }
        else if((plane.a == 0 && plane.c == 0) || plane.c == 0) { //projection to XZ
            p1.x = vertices[0].x, p1.y = vertices[0].z;
            p2.x = vertices[1].x, p2.y = vertices[1].z;
            p3.x = vertices[2].x, p3.y = vertices[2].z;
        }
        else if((plane.a == 0 && plane.b == 0) || plane.b == 0) { //projection to XY
            p1.x = vertices[0].x, p1.y = vertices[0].y;
            p2.x = vertices[1].x, p2.y = vertices[1].y;
            p3.x = vertices[2].x, p3.y = vertices[2].y;
        }
        else { //projection to XY
            p1.x = vertices[0].x, p1.y = vertices[0].y;
            p2.x = vertices[1].x, p2.y = vertices[1].y;
            p3.x = vertices[2].x, p3.y = vertices[2].y;
        }
        return Triangle2d(p1, p2, p3);
    }

    bool separable_plane_from(const Triangle3d& another) const {
        if(isParallel(another)) {
            if(isSamePlane(another)) { //2D intersection
                auto inter = intersect(another);
                return inter;
            }
            return false;
        }
        else { //3D intersection
            auto half_space = another.plane.get_side_area(vertices[0]);
            for (size_t i = 1; i < another.vertices.size(); i++) {
                auto another_space = another.plane.get_side_area(vertices[i]);
                if(another_space != INTER_SIDE) {
                    auto space = half_space^another_space;
                    if(space) return true;
                }
            }
            return false;
        }
    }
};

template <typename T>
std::vector<Triangle3d> createTriangles(const std::vector<T>& points) {
    std::vector<Triangle3d> triangles;
    triangles.reserve(points.size()/3);
    for(auto i = 0; i < points.size()-8;)
    {
        auto x1 = points[i++], y1 = points[i++], z1 = points[i++];
        auto x2 = points[i++], y2 = points[i++], z2 = points[i++];
        auto x3 = points[i++], y3 = points[i++], z3 = points[i++];

        triangles.emplace_back(Triangle3d(Point3d(x1, y1, z1), Point3d(x2, y2, z2), Point3d(x3, y3, z3)));
    }
    return triangles;
}

std::optional<std::vector<size_t>> intersectTriangles(const std::vector<Triangle3d>& triangles) {
    std::vector<size_t> intersection;
    intersection.reserve(triangles.size());
     for(auto i = 0; i < triangles.size()-1; ++i) {
        for(auto j = 1; j < triangles.size(); ++j) {
            if (triangles.at(i).separable_plane_from(triangles.at(j)))
            {
                intersection.push_back(i);
                break;
            }
        }
    }
    if(!intersection.empty())
        return intersection;
    else return std::nullopt;
}
