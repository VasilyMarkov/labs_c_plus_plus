#pragma once
#include <iostream>
#include <vector>
#include <cmath>

const constexpr float fit_tolerance  = 0.0001;

class Point {
private:
    float x_ = NAN;
    float y_ = NAN;
    float z_ = NAN;
public:
    Point(float x, float y, float z):x_(x), y_(y), z_(z) {}
    void print() const {
        std::cout << '(' << x_ << ", " << y_ << ", " << z_ << ')';
    }
    bool equal(const Point& rhs) const {
        return (std::abs(x_-rhs.x_) < fit_tolerance) &&
               (std::abs(y_-rhs.y_) < fit_tolerance) &&
               (std::abs(z_-rhs.z_) < fit_tolerance);
    }
};

class Line {
private:
    Point vector_;
    Point point_;
    enum class Side {LEFT, INTER, RIGHT};
public:
    Line(const Point& point, const Point& vector):vector_(vector), point_(point) {}
    Point intersectionPoint(const Line& rhs) const {

    }
};

class Polygon {
private:
    std::vector<Point*> vertices;

public:
    void print() const {
        std::cout << "Polygon: ";
        for(const auto& i : vertices) {
            i->print();
            std::cout << "; ";
        }
        std::cout << std::endl;
    }
    void square() {

    }
};
