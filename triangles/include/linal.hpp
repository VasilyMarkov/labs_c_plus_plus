#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <optional>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <span>

const double eps  = 1e-7;

bool equal(double a, double b) {
    return std::abs(a-b) < eps;
}
bool lessEqual(double a, double b) {
    return std::abs(a-b) <= eps;
}
bool greaterEqual(double a, double b) {
    return std::abs(a-b) >= eps;
}

struct Point2d {
protected:
    std::vector<double> v = {NAN, NAN, NAN};
public:
    Point2d() = default;
    Point2d(const Point2d& p) {v[0] = p.x(); v[1] = p.y();}
    Point2d(double x, double y) { v[0] = x; v[1] = y;}
    double x() const {return v[0];}
    double y() const {return v[1];}
    void setX(double x) {v[0] = x;}
    void setY(double y) {v[1] = y;}
    
    bool operator==(const Point2d& rhs) const {
        return equal(v[0], rhs.x()) && equal(v[1], rhs.y());
    }

    friend Point2d operator-(const Point2d& lhs, const Point2d& rhs) {
        return {lhs.v[0] - rhs.v[0], lhs.v[1] - rhs.v[1]};
    }

    friend std::ostream& operator<< (std::ostream& out, const Point2d& p) {
        out << "Point2d: x = " << p.v[0] << ", y = " << p.v[1];
        return out;
    }
};

struct Point3d: public Point2d {
public:
    Point3d() = default;
    Point3d(const Point2d& p) {v[0] = p.x(); v[1] = p.y();}
    Point3d(const Point3d& p) {v[0] = p.x(); v[1] = p.y(); v[2] = p.z();}
    Point3d(double x, double y, double z) { v[0] = x; v[1] = y; v[2] = z;}
    double z() const {return v[2];}
    void setZ(double z) {v[2] = z;}

    Point3d& operator=(const Point3d& rhs) {
        v[0] = rhs.v[0];
        v[1] = rhs.v[1];
        v[2] = rhs.v[2];
        return *this;
    }

    bool operator==(const Point3d& rhs) const {
        return equal(v[0], rhs.v[0]) && equal(v[1], rhs.v[1]) && equal(v[2], rhs.v[2]);
    }

    friend Point3d operator-(const Point3d& lhs, const Point3d& rhs) {
        return {lhs.v[0] - rhs.v[0], lhs.v[1] - rhs.v[1], lhs.v[2] - rhs.v[2]};
    }

    friend std::ostream& operator<< (std::ostream& out, const Point3d& p) {
        out << "Point3d: x = " << p.v[0] << ", y = " << p.v[1] << ", z = " << p.v[2] << ' ';
        return out;
    }

    static double dot(const Point3d& v1, const Point3d& v2) {
        return v1.x()*v2.x()+v1.y()*v2.y()+v1.z()*v2.z();
    }
    static Point3d cross(const Point3d& v1, const Point3d& v2) {
        return {v1.y()*v2.z()-v1.z()*v2.y(), -v1.x()*v2.z()+v1.z()*v2.x(), v1.x()*v2.y()-v1.y()*v2.x()};
    }

};

struct Plane {
    std::array<double ,3> coeffs = {0,0,0};
    Plane() = default;
    Plane(const Point3d& p1, const Point3d& p2, const Point3d& p3) {
        const auto vec1 = p2 - p1;
        const auto vec2 = p3 - p1;
        const auto cross = Point3d::cross(vec1, vec2);
        coeffs[0] = cross.x();
        coeffs[1] = cross.y();
        coeffs[2] = cross.z();
    }
    Plane(const Point2d& p1, const Point2d& p2, const Point2d& p3) {
        const auto vec1 = p2 - p1;
        const auto vec2 = p3 - p1;
        const auto cross = Point3d::cross({vec1.x(), vec1.y(), 0}, {vec2.x(), vec2.y(), 0});
        coeffs[0] = cross.x();
        coeffs[1] = cross.y();
        coeffs[2] = cross.z();
    }
};

std::ostream& operator<< (std::ostream& out, const Plane& p) {
    out << "Plane: x = " << p.coeffs[0] << ", y = " << p.coeffs[1] 
        << ", z = " << p.coeffs[2] << ' ';
    return out;
}

enum class position {
    same_half_space,
    diff_half_space,
    same_plane
};

position checkRelativePosition(const std::vector<double>& dets) {
    auto positive = false;
    auto negative = false;
    auto zero = false;
    for(auto i = 0; i < dets.size(); ++i) {
        if(dets.at(i) == 0)
            zero = true;
        else if(dets.at(i) > eps)
            positive = true;
        else if(dets.at(i) < -eps)
            negative = true;
    }
    if(zero && !positive && !negative)
        return position::same_plane;
    else if (!zero && positive != negative)
        return position::same_half_space;
    else
        return position::diff_half_space;
}

struct Triangle2d {
private:
    
public:
    Plane plane;
    std::array<Point2d, 3> vertices;

    Triangle2d() = default;
    Triangle2d(const Point2d& p1, const Point2d& p2, const Point2d& p3) {
        vertices[0] = p1, vertices[1] = p2, vertices[2] = p3;
    }
    Triangle2d(const std::array<Point2d, 3>& points): vertices(points), plane(points[0], points[1], points[2]) {}

    bool operator==(const Triangle2d& another) {
        return vertices[0] == another.vertices[0] &&
               vertices[1] == another.vertices[1] &&
               vertices[2] == another.vertices[2];
    }

    bool isPlane() const {
        if(!plane.coeffs[0] && !plane.coeffs[1] && !plane.coeffs[2]) return false;
        else return true;
    }

    bool pointInTriangle(const Point2d& pt) const
    {
        auto sign = [](const Point2d& p1, const Point2d& p2, const Point2d& p3) 
        {
            return (p1.x() - p3.x()) * (p2.y() - p3.y()) - (p2.x() - p3.x()) * (p1.y() - p3.y());
        };

        const auto d1 = sign(pt, vertices[0], vertices[1]);
        const auto d2 = sign(pt, vertices[1], vertices[2]);
        const auto d3 = sign(pt, vertices[2], vertices[0]);

        auto has_neg = false, has_pos = false;
        has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

        return !(has_neg && has_pos);
    }
};

std::ostream& operator<< (std::ostream& out, const Triangle2d& t) {
    out << "Triangle2d: " << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2] << ' ';  
    return out;
}

struct Triangle3d {
private:
    
public:
    Plane plane;
    std::array<Point3d, 3> vertices;
    Triangle3d() = default;
    Triangle3d(const Point3d& p1, const Point3d& p2, const Point3d& p3): vertices{p1, p2, p3}, plane(p1, p2, p3) {}
    Triangle3d(const std::array<Point3d, 3>& points): vertices(points), plane(points[0], points[1], points[2]) {}

    double det3(Point3d a, Point3d b, Point3d c) const {
        auto det = a.x()*(b.y()*c.z()-b.z()*c.y())-a.y()*(b.x()*c.z()-b.z()*c.x())+a.z()*(b.x()*c.y()-b.y()*c.x());
        return det;
    }

    double det4(Point3d a, Point3d b, Point3d c, Point3d d) const {
        auto det = (a.x()-b.x())*(c.y()*d.z()-c.z()*d.y())+
                   (b.y()-a.y())*(c.x()*d.z()-c.z()*d.x())+
                   (a.z()-b.z())*(c.x()*d.y()-c.y()*d.x())-
                    a.x()*(b.y()*d.z()-b.z()*d.y())+a.y()*(b.x()*d.z()-b.z()*d.x())-
                    a.z()*(b.x()*d.y()-b.y()*d.x())+det3(a,b,c);
        return det;
    }

    Point3d crossProduct(const Point3d& p1, const Point3d& p2) const {
        Point3d cross(p1.y()*p2.z()-p1.z()*p1.y(),
                      p1.z()*p2.x()-p1.x()*p1.z(),
                      p1.x()*p2.y()-p1.y()*p1.x());
        return cross;
    }

    enum class side_t {one_side, second_side, on_line};

    side_t get_side(const Point3d& line, const Point3d& point) const {
        auto cross = Point3d::cross(line, point);
        auto side = side_t::on_line;
        if(cross.z() > 0) side = side_t::one_side;
        else if(cross.z() < 0) side = side_t::second_side;
        return side;
    }

    enum class basis_plane {XY,XZ,YZ};

    template<size_t Size>
    std::array<Point2d, Size> projection(const std::array<Point3d, Size>& vertices) const {

        auto projection = [](const std::array<Point3d, Size>& vertices, basis_plane basis) ->
        std::array<Point2d, Size>
        {
            if(vertices.empty()) return {};

            std::array<Point2d, Size> out_vertices;

            for(auto i = 0; i < vertices.size(); ++i) {
                switch (basis)
                {
                case basis_plane::XY:
                    out_vertices[i].setX(vertices[i].x()); out_vertices[i].setY(vertices[i].y());
                    break;
                case basis_plane::XZ:
                    out_vertices[i].setX(vertices[i].x()); out_vertices[i].setY(vertices[i].z());
                    break;
                case basis_plane::YZ:
                    out_vertices[i].setX(vertices[i].y()); out_vertices[i].setY(vertices[i].z());
                    break;
                default:
                    break;
                }
            }
            return out_vertices;
        };

        if(plane.coeffs[1] == 0 && plane.coeffs[2] == 0) { //projection to YZ
            return projection(vertices, basis_plane::YZ);
        }
        else if((plane.coeffs[0] == 0 && plane.coeffs[2] == 0) || plane.coeffs[2] == 0) { //projection to XZ
            return projection(vertices, basis_plane::XZ);
        }
        else { //projection to XY
            return projection(vertices, basis_plane::XY);
        }
    }

    bool isPlane() const {
        if(!plane.coeffs[0] && !plane.coeffs[1] && !plane.coeffs[2]) return false;
        return true;
    }

    bool pointInTriangle(const Point3d& pt) const
    {
        const auto a = vertices[0] - pt;
        const auto b = vertices[1] - pt;
        const auto c = vertices[2] - pt;
        const auto u = Point3d::cross(a, b);
        const auto v = Point3d::cross(c, a);
        const auto w = Point3d::cross(b, c);
        if(Point3d::dot(u ,v) < 0) return false;
        if(Point3d::dot(u ,w) < 0) return false;
        return true;
    }

    //https://blackpawn.com/texts/pointinpoly/default.html Same Side Technique
    bool separable_line_from(const Triangle2d& thisProj, const Triangle2d& anotherProj) const {

        std::unordered_set<side_t> sides;
        for (size_t i = 0; i < thisProj.vertices.size(); ++i) {

            const auto line_point2 = thisProj.vertices[(i+1) % thisProj.vertices.size()];
            const auto line_point1 = thisProj.vertices[i % thisProj.vertices.size()];

            for (size_t j = 0; j < anotherProj.vertices.size(); ++j) {
                const auto point2 = anotherProj.vertices[j % anotherProj.vertices.size()];
                const auto point1 = thisProj.vertices[i % thisProj.vertices.size()];

                const auto side = get_side(line_point2-line_point1, point2 - point1);

                const auto interval_len = std::hypot(line_point2.x()-line_point1.x(), line_point2.y()-line_point1.y());
                const auto half_sum1 = std::hypot(point2.x()-line_point1.x(), point2.y()-line_point1.y());
                const auto half_sum2 = std::hypot(point2.x()-line_point2.x(), point2.y()-line_point2.y());
                const auto is_point_inside = equal(half_sum1+half_sum2, interval_len);

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

    //https://inria.hal.science/inria-00072100/file/RR-4488.pdf
    bool separable_plane_from(const Triangle3d& another) const {
        
        if (!isPlane() || !another.isPlane()) return false;

        std::vector<double> signs_this, signs_another;

        for (size_t i = 0; i < another.vertices.size(); ++i) {  
            auto det_this = det4(vertices[0], vertices[1], vertices[2], another.vertices[i]);
            auto det_another = det4(another.vertices[0], another.vertices[1], another.vertices[2], vertices[i]);
    
            signs_this.push_back(det_this);
            signs_another.push_back(det_another);

        }
        auto pos1 = checkRelativePosition(signs_this);
        const Triangle2d thisProj(projection(vertices));
        const Triangle2d anotherProj(projection(another.vertices));
        auto zeros1 = 0, zeros2 = 0;
        auto zeros_index = 0;
        for(auto i = 0; i < signs_this.size(); ++i) {
            if(signs_this[i] == 0) {
                zeros_index = i;
                ++zeros1;
            }
        }
        for(auto i = 0; i < signs_another.size(); ++i) {
            if(signs_another[i] == 0) ++zeros2;
        }
        if(((zeros1 == 1 && zeros2 == 2) || (zeros1 == 2 && zeros2 == 1)) || 
            (zeros1 == 1 && zeros2 == 1)) {
            if(!pointInTriangle(another.vertices[zeros_index])) return false;
        }

        if(pos1 == position::same_plane) {
            auto res = separable_line_from(thisProj, anotherProj);
            return res;
        }
        else {

            auto pos2 = checkRelativePosition(signs_another);
            if(pos1 == position::same_half_space && pos2 == position::same_half_space) {
                return false;
            }
            else {
                if(pos1 == position::diff_half_space && pos2 == position::diff_half_space) {

                }
                return true;
            }
        }

    }

};

std::ostream& operator<< (std::ostream& out, const Triangle3d& t) {
    out << "Triangle3d: " << t.vertices[0] << ", " << t.vertices[1] << ", " << t.vertices[2];  
    return out;
}

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
        for(auto j = 1; j < triangles.size(); ++j) {
            if(i != j) {
                auto res = triangles[i].separable_plane_from(triangles[j]);
                if (res) {
                    intersection.emplace(i);
                    intersection.emplace(j);
                }
            }
        }
        k++;
    }
    if(!intersection.empty())
        return intersection;
    else return std::nullopt;
}
