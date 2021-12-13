/*
--- Part Two ---
Finish folding the transparent paper according to the instructions. The manual says the code is always eight capital letters.

What code do you use to activate the infrared thermal imaging camera system?
*/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cctype>
#include <list>

using Cave = std::string;

bool big(Cave cave) {
  return std::isupper(cave[0]);
}

bool small(Cave cave) {
  return std::islower(cave[0]);
}

auto identity = [](auto s) {
    return s; 
};

template<typename T>
std::vector<T> split(std::string line, const std::string& delim, auto toT(std::string) -> T) {
  std::vector<T> out;
  for (;;) {
    if(auto delim_position = line.find(delim);
          delim_position != std::string::npos) {
      out.push_back(toT(line.substr(0, delim_position)));
      line = line.substr(delim_position + delim.size());
    } else {
    break;
    }
  }
  out.push_back(toT(line));
  return out;
}

template<typename T>
std::vector<T> split(std::string line, const std::string& delim) {
  return split<T>(line, delim, identity);
}

template<typename T>
std::vector<T> split(std::string line) {
  return split<T>(line, " ");
}

using Point = std::pair<int, int>;

enum Axis {
  X,
  Y
};

struct Fold {
  Axis axis;
  int along;
};

void fold_left(int along, std::vector<Point>& points) {
  for (auto& point: points) {
    if (point.first > along)
      point.first = 2 * along - point.first;
  }
}

void fold_up(int along, std::vector<Point>& points) {
  for (auto& point: points) {
    if (point.second > along)
      point.second = 2 * along - point.second;
  }
}
template<typename T>
std::pair<Point, Point> grid(const T& points) {
  auto [minx, maxx] = std::minmax_element(std::begin(points), std::end(points), [](const Point& a, const Point& b) {
    return a.first < b.first;
  });
  auto [miny, maxy] = std::minmax_element(std::begin(points), std::end(points), [](const Point& a, const Point& b) {
    return a.second < b.second;
  });
  
  return {{minx->first, miny->second}, {maxx->first, maxy->second}};
}

int main() {     
  std::vector<Point> points;
  for (std::string line; std::getline(std::cin, line) && !line.empty();) {
    auto coordinates = split<int>(line, ",", [](auto s) { return std::stoi(s); });
    assert(coordinates.size() == 2);
    points.emplace_back(coordinates[0], coordinates[1]);
  }

  std::vector<Fold> folds;
  for (std::string line; std::getline(std::cin, line);) {
    auto fold_strs = split<std::string>(line.substr(11), "=");
    assert(fold_strs.size() == 2);
    folds.emplace_back(fold_strs[0] == "x" ? X : Y, std::stoi(fold_strs[1]));
  }

  for (auto& [axis, along]: folds) {
    if (axis == X) {
      fold_left(along, points);
    } else {
      fold_up(along, points);
    }
  }
  std::set<Point> unique(points.begin(), points.end());

  auto [left_top, right_bottom] = grid(unique);
  for (int y = left_top.second; y <= right_bottom.second; ++y) {
    for (int x = left_top.first; x <= right_bottom.first; ++x) {
      if (unique.contains({x, y})) {
        std::cout << '#';
      } else {
        std::cout << '.';
      }
    }
    std::cout << "\n";
  }
}