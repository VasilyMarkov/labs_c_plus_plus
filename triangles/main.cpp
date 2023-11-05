#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>
template <typename T>
void print(const std::vector<T>& vec) {
    for(auto const& i : vec) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}

template <typename T, typename U>
void print(const std::multimap<T, U>& vec) {
    for(auto& [key, value] : vec) {
        std::cout << key << ' ' << value << std::endl;
    }
}

template <typename T>
void divideByAxis(const std::vector<T>& triangles, std::vector<T>& x_axis, std::vector<T>& y_axis) {
    for(auto i = 0, cnt_x = 0, cnt_y = 0; i < triangles.size(); ++i) {
        if(i % 2) {
            y_axis.emplace_back(triangles[i]);
        }
        else {
            x_axis.emplace_back(triangles[i]);
        }
    }
    return;
}

template <typename T, typename U>
void sortingPoints(const std::vector<T>& x_axis, const std::vector<T>& y_axis,
                   std::multimap<T, U>& x_sort, std::multimap<T, U>& y_sort) {
    std::vector<int> x_projection(3), y_projection(3);
    for(auto i = 0, cnt = 1; i < x_axis.size(); ++i) {
        x_projection[i % 3] = x_axis[i];
        y_projection[i % 3] = y_axis[i];
        if(!(cnt % 3)) {
            auto min = *std::min_element(std::begin(x_projection), std::end(x_projection));
            auto max = *std::max_element(std::begin(x_projection), std::end(x_projection));
            x_sort.emplace(min, cnt/3);
            x_sort.emplace(max, cnt/3);
            min = *std::min_element(std::begin(y_projection), std::end(y_projection));
            max = *std::max_element(std::begin(y_projection), std::end(y_projection));
            y_sort.emplace(min, cnt/3);
            y_sort.emplace(max, cnt/3);
        }
        ++cnt;
    }
}

template <typename T, typename U>
void intersectByAxis(std::multimap<T, U>& points, std::vector<U>& triangles_index) {
    std::unordered_set<size_t> remove;
    auto this_it = std::begin(points);
    auto other_it = std::next(std::begin(points));
    while(this_it != std::end(points)) {
        if(remove.find(this_it->second) != std::end(remove)) {
            points.erase(this_it);
            remove.erase(this_it->second);
            if(points.size() == 0)
                break;
            ++this_it;
            continue;
        }
        if(other_it->second != this_it->second) {
            triangles_index.push_back(this_it->second);
            triangles_index.push_back(other_it->second);
            points.erase(this_it);
            points.erase(other_it);
            remove.emplace(this_it->second);
            remove.emplace(other_it->second);
            this_it = std::begin(points);
            other_it = std::next(std::begin(points));
        }
        else {
            auto next_it = std::next(this_it);
            points.erase(this_it);
            points.erase(next_it);
            if(points.size() == 0)
                break;
            this_it = std::begin(points);
            other_it = std::next(std::begin(points));
        }
        if(other_it == std::end(points))
            ++this_it;
    }
    return;
}

template <typename T>
std::vector<T> axisesCompare(std::vector<T> x_axis, std::vector<T> y_axis) {
    std::sort(std::begin(x_axis), std::end(x_axis));
    std::sort(std::begin(y_axis), std::end(y_axis));

    std::vector<T> intersection;
    std::set_intersection(std::begin(x_axis), std::end(x_axis), std::begin(y_axis), std::end(y_axis),
                              std::back_inserter(intersection));
    return intersection;
}

int main() {
    size_t n = 2;
//    std::vector<int> trianle_set = {0,0, 3,0, 3,3, 1,0, 4,0, 4,3};
    std::vector<int> trianle_set = {0,0, 3,0, 3,3, 4,0, 7,0, 7,3};
    std::vector<int> x_axis, y_axis;

    std::multimap<int, size_t> x_points, y_points;

    divideByAxis(trianle_set, x_axis, y_axis);
    sortingPoints(x_axis, y_axis, x_points, y_points);

    std::vector<size_t> x_triangles, y_triangles;
    intersectByAxis(x_points, x_triangles);
    intersectByAxis(y_points, y_triangles);

//    print(x_triangles);
//    print(y_triangles);
    print(axisesCompare(x_triangles, y_triangles));
    return 0;
}
