/*
--- Day 9: Smoke Basin ---
These caves seem to be lava tubes. Parts are even still volcanically active; small hydrothermal vents release smoke into the caves that slowly settles like rain.

If you can model how the smoke flows through the caves, you might be able to avoid it and be that much safer. The submarine generates a heightmap of the floor of the nearby caves for you (your puzzle input).

Smoke flows to the lowest point of the area it's in. For example, consider the following heightmap:

2199943210
3987894921
9856789892
8767896789
9899965678
Each number corresponds to the height of a particular location, where 9 is the highest and 0 is the lowest a location can be.

Your first goal is to find the low points - the locations that are lower than any of its adjacent locations. Most locations have four adjacent locations (up, down, left, and right); locations on the edge or corner of the map have three or two adjacent locations, respectively. (Diagonal locations do not count as adjacent.)

In the above example, there are four low points, all highlighted: two are in the first row (a 1 and a 0), one is in the third row (a 5), and one is in the bottom row (also a 5). All other locations on the heightmap have some lower adjacent location, and so are not low points.

The risk level of a low point is 1 plus its height. In the above example, the risk levels of the low points are 2, 1, 6, and 6. The sum of the risk levels of all low points in the heightmap is therefore 15.

Find all of the low points on your heightmap. What is the sum of the risk levels of all low points on your heightmap?
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

template<typename T>
std::set<T> diff(const std::set<T>& first_set, const std::set<T>& second_set) {
  std::set<T> out;
  std:set_difference(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), std::inserter(out, out.begin()));
  return out;
}

int main() {     
  std::vector<std::vector<int>> heightmap;
  for (std::string line; std::getline(std::cin, line);) {
    heightmap.push_back({});
    heightmap.back().reserve(line.size());
    for (auto c: line) {
      heightmap.back().push_back(c - '0');
    }
  }

  int risk = 0;

  for (int i = 0; i < heightmap.size(); ++i) {
    for (int j = 0; j < heightmap[i].size(); ++j) {
      bool top = i == 0;
      bool bottom = i == heightmap.size() - 1;
      bool left = j == 0;
      bool right = j == heightmap[j].size() - 1;

      if (left) {
        if (top) {
          if (heightmap[0][0] < heightmap[1][0] && heightmap[0][0] < heightmap[0][1]) {
            risk += 1 + heightmap[0][0];
          }
        } else if (bottom) {
          if (heightmap[i][0] < heightmap[i-1][0] && heightmap[i][0] < heightmap[i][1]) {
            risk += 1 + heightmap[i][0];
          }
        } else {
          if (heightmap[i][0] < heightmap[i+1][0] && heightmap[i][0] < heightmap[i][1] && heightmap[i][0] < heightmap[i-1][0]) {
            risk += 1 + heightmap[i][0];
          }
        }
      } else if (right) {
        if (top) {
          if (heightmap[0][j] < heightmap[1][j] && heightmap[0][j] < heightmap[0][j-1]) {
            risk += 1 + heightmap[0][j];
          }
        } else if (bottom) {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1]) {
            risk += 1 + heightmap[i][j];
          }
        } else {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1] && heightmap[i][j] < heightmap[i+1][j]) {
            risk += 1 + heightmap[i][j];
          }
        }        
      } else {
        if (top) {
          if (heightmap[0][j] < heightmap[0][j-1] && heightmap[0][j] < heightmap[0][j+1] && heightmap[0][j] < heightmap[1][j]) {
            risk += 1 + heightmap[0][j];
          }
        } else if (bottom) {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1] && heightmap[i][j] < heightmap[i][j+1]) {
            risk += 1 + heightmap[i][j];
          }
        } else {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1] && heightmap[i][j] < heightmap[i][j+1] && heightmap[i][j] < heightmap[i+1][j]) {
            risk += 1 + heightmap[i][j];
          }
      }
    }
  }
  }

  std::cout << risk << std::endl;
 
}