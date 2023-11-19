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

struct Point {
    float x = NAN;
    float y = NAN;
    float z = NAN;
    Point() = default;
    Point(float x_i, float y_i, float z_i): x(x_i), y(y_i), z(z_i){}
    bool valid() const { return !(x != x || y != y); }

    void print() const {
        std::cout << '(' << x << ", " << y << ", " << ')';
    }
    bool equal(const Point& rhs) const {
        return (std::abs(x-rhs.x) < fit_tolerance) &&
               (std::abs(y-rhs.y) < fit_tolerance) &&
               (std::abs(z-rhs.z) < fit_tolerance);
    }
};

struct Line {
    float a = -1.0f, b = 1.0f, c = 0.0f;

    Line(const Point &p1, const Point &p2) {
        float angle = std::atan((p2.y - p1.y) / (p2.x - p1.x));
        float sin_angle = std::sin(angle);
        float cos_angle = std::sqrt(1.0 - sin_angle * sin_angle);
        Point normal_vect{-sin_angle, cos_angle, 0};
        a = normal_vect.x;
        b = normal_vect.y;
        c = -(p1.x * normal_vect.x + p1.y * normal_vect.y);
    }

    enum area_t get_side_area(const Point& point) const {
        float side_offset = a * point.x + b * point.y + c;
        if (side_offset > 0.0 + fit_tolerance * inter_area_width)
          return RIGHT_SIDE;

        if (side_offset > 0.0 - fit_tolerance * inter_area_width)
          return INTER_SIDE;

        return LEFT_SIDE;
    }

    bool separates(const Point &pnt1, const Point &pnt2) const {
        enum area_t side1 = get_side_area(pnt1);
        enum area_t side2 = get_side_area(pnt2);
        if (side1 == INTER_SIDE || side2 == INTER_SIDE)
          return false;
        return !(side1 == side2);
    }

    bool valid() const { return !(a != a || b != b || c != c); }

    bool intersect(const Line &another) const {
        return (std::abs(a * another.b - another.a * b) >= fit_tolerance);
    }

    Point point_of_intersect(const Line &another) const {
        if (!intersect(another))
          return Point{};
        float det = (a * another.b - another.a * b);
        float det1 = ((-c) * another.b - (-another.c) * b);
        float det2 = (a * (-another.c) - another.a * (-c));
        return {det1 / det, det2 / det, 0};
    }
};


struct Triangle {
private:
    struct Plane {
        float a = NAN, b = NAN, c = NAN, d = NAN;
        Plane(const Point& p1, const Point& p2, const Point& p3) {
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
        enum area_t get_side_area(const Point& point) const {
            float side_offset = a * point.x + b * point.y + c * point.z + d;
            auto treshold = fit_tolerance * inter_area_width;
            if (side_offset > 0.0 + treshold)
              return RIGHT_SIDE;

            if (side_offset > 0.0 - treshold)
              return INTER_SIDE;

            return LEFT_SIDE;
        }
    };
    Plane plane;
    std::array<Point, 3> vertices;
public:
    Triangle(const Point& p1, const Point& p2, const Point& p3): plane(p1, p2, p3) {
        vertices[0] = p1, vertices[1] = p2, vertices[2] = p3;

    }

    Point crossProduct(const Point& p1, const Point& p2) const {
        Point cross(p1.y*p2.z-p1.z*p1.y,
                    p1.z*p2.x-p1.x*p1.z,
                    p1.x*p2.y-p1.y*p1.x);
        return cross;
    }

    bool isParallel(const Triangle& another) const {

        auto cross = crossProduct(Point(plane.a, plane.b, plane.c),
                                  Point(another.plane.a, another.plane.b, another.plane.c));
        if(cross.x == 0 && cross.y == 0 && cross.z == 0 ) return true;

//        auto a_div = 0, b_div = 0, c_div = 0;
//        if(another.plane.a != 0) a_div = plane.a/another.plane.a;
//        if(another.plane.b != 0) b_div = plane.b/another.plane.b;
//        if(another.plane.c != 0) c_div = plane.c/another.plane.c;

//        auto eps = fit_tolerance*inter_area_width;
//        if(std::abs(a_div-b_div) <= eps && std::abs(a_div-c_div) <= eps) return true;
        return false;
    }

    bool isSamePlane(const Triangle& another) const{
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

    Line get_side(int index) const {
        assert(!vertices.empty());
        auto vsz = vertices.size();
        if (index < 0)
        index += (1 + (index / vsz)) * vsz;
        Line ret(vertices[index % vsz], vertices[(index + 1) % vsz]);
        return ret;
    }

    const bool noIntersect = false;
    const bool intersect = true;

    bool separable_line_from(const Triangle& another) const {
        for (size_t i = 0; i < vertices.size(); i++) {
          Line div_line = get_side(i);

          enum area_t half_space =
              div_line.get_side_area(vertices[(i + 2) % vertices.size()]);

          if (half_space == INTER_SIDE)
            continue;

          bool flag = noIntersect;
          for (size_t j = 0; j < another.vertices.size(); j++) {
            Point vertex = another.vertices[j];
            if (half_space == div_line.get_side_area(vertex)) {
              flag = noIntersect;
              break;
            }
          }
          if (flag)
            return intersect;
        }
        return noIntersect;
    }

    bool separable_plane_from(const Triangle& another) const {
        if(isParallel(another)) {
            if(isSamePlane(another)) { //2D intersection
                return separable_line_from(another);
            }
            return noIntersect;
        }
        else { //3D intersection
            auto half_space = another.plane.get_side_area(vertices[0]);
            for (size_t i = 1; i < another.vertices.size(); i++) {
                auto another_space = another.plane.get_side_area(vertices[i]);
                auto space = half_space^another_space;
                if(space) return intersect;
            }
            return noIntersect;
        }
    }
};

template <typename T>
std::vector<Triangle> createTriangles(const std::vector<T>& points) {
    std::vector<Triangle> triangles;
    triangles.reserve(points.size()/3);
    for(auto i = 0; i < points.size()-8;)
    {
        auto x1 = points[i++], y1 = points[i++], z1 = points[i++];
        auto x2 = points[i++], y2 = points[i++], z2 = points[i++];
        auto x3 = points[i++], y3 = points[i++], z3 = points[i++];

        triangles.emplace_back(Triangle(Point(x1, y1, z1), Point(x2, y2, z2), Point(x3, y3, z3)));
    }
    return triangles;
}

std::optional<std::vector<size_t>> intersectTriangles(const std::vector<Triangle>& triangles) {
    std::vector<size_t> intersection;
    intersection.reserve(triangles.size());
    for(auto i = 0; i < triangles.size(); ++i) {
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
