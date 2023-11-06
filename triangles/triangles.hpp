#pragma once
#include <vector>
#include <map>
#include <unordered_set>

namespace prep {
template <typename T>
void divideByAxis(const std::vector<T>& triangles, std::vector<T>& x_axis, std::vector<T>& y_axis, std::vector<T>& z_axis) {

    for(auto i = 0; i < triangles.size()-2;)
    {
        x_axis.emplace_back(triangles[i++]);
        y_axis.emplace_back(triangles[i++]);
        z_axis.emplace_back(triangles[i++]);
    }

    return;
}

template <typename T, typename U>
void sortingPoints(const std::vector<T>& x_axis, const std::vector<T>& y_axis, const std::vector<T>& z_axis,
                   std::multimap<T, U>& x_sort, std::multimap<T, U>& y_sort, std::multimap<T, U>& z_sort) {

    std::vector<T> x_buf(3), y_buf(3), z_buf(3);

    auto createSortPoints {[](std::multimap<T, U>& map, const std::vector<T>& buf, T n){
        auto min = *std::min_element(std::begin(buf), std::end(buf));
        auto max = *std::max_element(std::begin(buf), std::end(buf));
        map.emplace(min, n/3);
        map.emplace(max, n/3);

    }};

    for(auto i = 0, cnt = 1; i < x_axis.size(); ++i) {
        x_buf[i % 3] = x_axis[i];
        y_buf[i % 3] = y_axis[i];
        z_buf[i % 3] = z_axis[i];
        if(!(cnt % 3)) {
            createSortPoints(x_sort, x_buf, cnt);
            createSortPoints(y_sort, y_buf, cnt);
            createSortPoints(z_sort, z_buf, cnt);
        }
        ++cnt;
    }
}
}

namespace intersect {
template <typename T, typename U>
void intersectByAxis(std::multimap<T, U>& points, std::vector<U>& triangles_index) {
    std::unordered_set<U> removed;
    std::unordered_set<U> index;
    auto this_it = std::begin(points);
    auto other_it = std::next(std::begin(points));
    while(this_it != std::end(points)) {
        if(removed.find(this_it->second) != std::end(removed)) {
            points.erase(this_it);
            removed.erase(this_it->second);
            if(points.size() == 0)
                break;
            this_it = std::begin(points);
            other_it = std::next(std::begin(points));
            continue;
        }
        if(other_it->second != this_it->second) {
            index.emplace(this_it->second);
            index.emplace(other_it->second);
            points.erase(this_it);
            points.erase(other_it);
            removed.emplace(this_it->second);
            removed.emplace(other_it->second);
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
//    std::copy(std::begin(index), std::end(index), std::begin(triangles_index));
    std::copy(index.begin(), index.end(), std::back_inserter(triangles_index));
    return;
}

template <typename T>
std::vector<T> axisesCompare(std::vector<T>& l_axis, std::vector<T>& r_axis) {
    std::sort(std::begin(l_axis), std::end(l_axis));
    std::sort(std::begin(r_axis), std::end(r_axis));

    std::vector<T> intersection;

    std::set_intersection(std::begin(l_axis), std::end(l_axis), std::begin(r_axis), std::end(r_axis),
                              std::back_inserter(intersection));
    return intersection;
}
}


