#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <optional>
#include <set>
#include <unordered_set>
const float fit_tolerance  = 0.00001;
const float inter_area_width = 100.0;


struct Point2d {
public:
    float x = NAN;
    float y = NAN;
public:
    Point2d() = default;
    Point2d(float x_i, float y_i): x(x_i), y(y_i){}
    Point2d(const Point2d& another): x(another.x), y(another.y){}
};

Point2d operator-(const Point2d& lhs, const Point2d& rhs) {
    Point2d sub = lhs;
    sub.x -= rhs.x;
    sub.y -= rhs.y;
    return sub;
}

struct Point3d: public Point2d {
    float z = NAN;
    Point3d() = default;
    Point3d(float x_i, float y_i, float z_i): Point2d(x_i, y_i), z(z_i){}
    Point3d(const Point3d& rhs): Point2d(rhs.x, rhs.y), z(rhs.z){}
    Point3d& operator=(const Point3d& rhs) {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Point3d& operator-(const Point3d& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    bool operator==(const Point3d& rhs) const {
        return (std::abs(x-rhs.x) < fit_tolerance) &&
               (std::abs(y-rhs.y) < fit_tolerance) &&
               (std::abs(z-rhs.z) < fit_tolerance);
    }

    bool valid() const { return !(x != x || y != y || z != z); }

    void print() const {
        std::cout << '(' << x << ", " << y << ", " << ')';
    }

    friend std::ostream& operator<< (std::ostream& out, const Point3d& p) {
        out << "Point3d: x = " << p.x << ", y = " << p.y << ", z = " << p.z << std::endl;
        return out;
    }

    bool operator!=(const Point3d& rhs) const {
        return !(*this == rhs);
    }
    static float dot(const Point3d& v1, const Point3d& v2) {
        return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
    }
    static Point3d cross(const Point3d& v1, const Point3d& v2) {
        return {v1.y*v2.z-v1.z*v2.y, -v1.x*v2.z+v1.z*v2.x, v1.x*v2.y-v1.y*v2.x};
    }
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
    friend std::ostream& operator<< (std::ostream& out, const Plane& p) {
        out << "Plane: a = " << p.a << ", b = " << p.b 
            << ", c = " << p.c << ", d = " << p.d << std::endl;
        return out;
    }
};


struct Triangle2d {
    std::array<Point2d, 3> vertices;
    Triangle2d() = default;
    Triangle2d(const Point2d& p1, const Point2d& p2, const Point2d& p3) {
        vertices[0] = p1, vertices[1] = p2, vertices[2] = p3;
    }
};

struct Triangle3d {
private:
    Plane plane;
    std::array<Point3d, 3> vertices;
    enum class position {
        same_half_space,
        diff_half_space,
        same_plane
    };

public:
    Triangle3d() = default;
    Triangle3d(const Point3d& p1, const Point3d& p2, const Point3d& p3): plane(p1, p2, p3) {
        vertices[0] = p1, vertices[1] = p2, vertices[2] = p3;
    }

    float det3(Point3d a, Point3d b, Point3d c) const {
        auto det = a.x*(b.y*c.z-b.z*c.y)-a.y*(b.x*c.z-b.z*c.x)+a.z*(b.x*c.y-b.y*c.x);
        return det;
    }

    float det4(Point3d a, Point3d b, Point3d c, Point3d d) const {
        auto det = (a.x-b.x)*(c.y*d.z-c.z*d.y)+(b.y-a.y)*(c.x*d.z-c.z*d.x)+(a.z-b.z)*(c.x*d.y-c.y*d.x)-
                         a.x*(b.y*d.z-b.z*d.y)+a.y*(b.x*d.z-b.z*d.x)-a.z*(b.x*d.y-b.y*d.x)+det3(a,b,c);
        return det;
    }

    Point3d crossProduct(const Point3d& p1, const Point3d& p2) const {
        Point3d cross(p1.y*p2.z-p1.z*p1.y,
                      p1.z*p2.x-p1.x*p1.z,
                      p1.x*p2.y-p1.y*p1.x);
        return cross;
    }

    enum class side_t {one_side, second_side, on_line};

    side_t get_side(const Point3d& line, const Point3d& point) const {
        auto cross = Point3d::cross(line, point);
        auto side = side_t::on_line;
        if(cross.z > 0) side = side_t::one_side;
        else if(cross.z < 0) side = side_t::second_side;
        return side;
    }

    //https://blackpawn.com/texts/pointinpoly/default.html Same Side Technique
    bool separable_line_from(const Triangle3d& another) const {
        auto anotherProj = another.triangleProjection();
        auto thisProj = triangleProjection();
        auto verts = thisProj.vertices;
        std::unordered_set<side_t> sides;
        for (size_t i = 0; i < thisProj.vertices.size(); ++i) {
            auto line_point2 = thisProj.vertices[(i+1) % verts.size()];
            auto line_point1 = thisProj.vertices[i % verts.size()];
            auto line  = line_point2-line_point1;
            Point3d line3d = {line.x, line.y, 0};
            for (size_t j = 0; j < anotherProj.vertices.size(); ++j) {

                Point3d point_vec = {anotherProj.vertices[j % anotherProj.vertices.size()].x,
                                     anotherProj.vertices[j % anotherProj.vertices.size()].y,
                                     0};
                auto point2 = anotherProj.vertices[j % anotherProj.vertices.size()];
                auto point1 = thisProj.vertices[i % thisProj.vertices.size()];
                auto vPoint = point2 - point1;
                Point3d vPoint3d = {vPoint.x, vPoint.y, 0};
                auto side = get_side(line3d, vPoint3d);

                auto is_point_inside = point2.x <= line_point2.x && point2.x >= line_point1.x &&
                                       point2.y <= line_point2.y && point2.y >= line_point1.y;
                if (side == side_t::on_line && is_point_inside)
                    return true;
                else if (side != side_t::on_line)
                    sides.insert(side);
            }
            if(sides.size() > 1) {
                return true;
            }
            sides.clear();
        }
        return false;
    }


    Triangle2d triangleProjection() const {
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

    position checkRelativePosition(const std::vector<int>& dets) const{
        if(std::all_of(std::begin(dets), std::end(dets), [](int i) { return i==0; })) {
            return position::same_plane;
        }
        else {
            bool same_sign = false;
            for(auto i = 1; i < dets.size(); ++i) {
                if(!(dets[i-1] >= 0) ^ (dets[i] < 0)) { //it's a diff sign?
                    return position::diff_half_space;
                }
            }
            return position::same_half_space;
        }
    }

    bool isPlane() const {
        if(plane.a == 0 && plane.b == 0 && plane.c == 0 && plane.d == 0) return false;
        return true;
    }

    //https://inria.hal.science/inria-00072100/file/RR-4488.pdf
    bool separable_plane_from(const Triangle3d& another) const {
        if (!isPlane()) return false;
        std::vector<int> signs_tr1, signs_tr2;
        for (size_t i = 0; i < another.vertices.size(); i++) {
            auto det_this = det4(vertices[0], vertices[1], vertices[2], another.vertices[i]);
            auto det_another = det4(another.vertices[0], another.vertices[1], another.vertices[2], vertices[i]);
            signs_tr1.push_back(det_this);
            signs_tr2.push_back(det_another);
        }
        auto pos1 = checkRelativePosition(signs_tr1);
        if(pos1 == position::same_plane) {
            return separable_line_from(another);
        }
        else {
           auto pos2 = checkRelativePosition(signs_tr2);
           if(pos1 == position::diff_half_space && pos2 == position::diff_half_space) {
               return true;
           }
           else {
               return false;
           }
        }

    }
    void print() const {
        std::cout << "Triangle: p1(" << vertices[0].x << ", " << vertices[0].y << ", " << vertices[0].z << "), p2("
                  << vertices[1].x << ", " << vertices[1].y << ", " << vertices[1].z << "), p3("
                  << vertices[2].x << ", " << vertices[2].y << ", " << vertices[2].z << ")" << std::endl;

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

std::optional<std::set<size_t>> intersectTriangles(const std::vector<Triangle3d>& triangles) {
    std::set<size_t> intersection;
    size_t k = 0;
     for(auto i = 0; i < triangles.size(); ++i) {
        for(auto j = k; j < triangles.size(); ++j) {
            if (triangles.at(i).separable_plane_from(triangles.at(j)) && i != j) {
                intersection.emplace(i);
                intersection.emplace(j);
            }
        }
        k++;
    }
    if(!intersection.empty())
        return intersection;
    else return std::nullopt;
}
