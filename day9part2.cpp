/*
--- Part Two ---
Next, you need to find the largest basins so you know what areas are most important to avoid.

A basin is all locations that eventually flow downward to a single low point. Therefore, every low point has a basin, although some basins are very small. Locations of height 9 do not count as being in any basin, and all other locations will always be part of exactly one basin.

The size of a basin is the number of locations within the basin, including the low point. The example above has four basins.

The top-left basin, size 3:

2199943210
3987894921
9856789892
8767896789
9899965678
The top-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678
The middle basin, size 14:

2199943210
3987894921
9856789892
8767896789
9899965678
The bottom-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678
Find the three largest basins and multiply their sizes together. In the above example, this is 9 * 14 * 9 = 1134.

What do you get if you multiply together the sizes of the three largest basins?
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

int count_to_9(std::set<std::pair<int, int>>&& visited, const std::vector<std::vector<int>>& heightmap, int i, int j) {
      if (visited.contains(std::pair<int, int>(i, j)))
        return 0;
      visited.insert(std::pair<int, int>(i,j));
      bool top = i == 0;
      bool bottom = i == heightmap.size() - 1;
      bool left = j == 0;
      bool right = j == heightmap[j].size() - 1;

      if (left) {
        if (top) {
          int toright = heightmap[i][j + 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j + 1) : 0;
          int todown = heightmap[i + 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i + 1, j) : 0;
          return 1 + toright + todown;
        } else if (bottom) {
          int toright = heightmap[i][j + 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j + 1) : 0;
          int toup = heightmap[i - 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i - 1, j) : 0;
          return 1 + toright + toup;
        } else {
          int toright = heightmap[i][j + 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j + 1) : 0;
          int toup = heightmap[i - 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i - 1, j) : 0;
          int todown = heightmap[i + 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i + 1, j) : 0;
          return 1 + toright + toup + todown;
        }
      } else if (right) {
        if (top) {
          int toleft = heightmap[i][j - 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j - 1) : 0;
          int todown = heightmap[i + 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i + 1, j) : 0;
          return 1 + toleft + todown;
        } else if (bottom) {
          int toleft = heightmap[i][j - 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j - 1) : 0;
          int toup = heightmap[i - 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i - 1, j) : 0;
          return 1 + toleft + toup;
        } else {
          int toleft = heightmap[i][j - 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j - 1) : 0;
          int toup = heightmap[i - 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i - 1, j) : 0;
          int todown = heightmap[i + 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i + 1, j) : 0;
          return 1 + toleft + toup + todown;
        }        
      } else {
        if (top) {
          int toleft = heightmap[i][j - 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j - 1) : 0;
          int toright = heightmap[i][j + 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j + 1) : 0;
          int todown = heightmap[i + 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i + 1, j) : 0;
          return 1 + toleft + toright + todown;
        } else if (bottom) {
          int toleft = heightmap[i][j - 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j - 1) : 0;
          int toright = heightmap[i][j + 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j + 1) : 0;
          int toup = heightmap[i - 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i - 1, j) : 0;
          return 1 + toleft + toright + toup;
        } else {
          int toleft = heightmap[i][j - 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j - 1) : 0;
          int toright = heightmap[i][j + 1] < 9 ? count_to_9(std::move(visited), heightmap, i, j + 1) : 0;
          int toup = heightmap[i - 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i - 1, j) : 0;
          int todown = heightmap[i + 1][j] < 9 ? count_to_9(std::move(visited), heightmap, i + 1, j) : 0;
          return 1 + toleft + toright + toup + todown;
      }
    }
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

  std::vector<int> basins;

  for (int i = 0; i < heightmap.size(); ++i) {
    for (int j = 0; j < heightmap[i].size(); ++j) {
      bool top = i == 0;
      bool bottom = i == heightmap.size() - 1;
      bool left = j == 0;
      bool right = j == heightmap[j].size() - 1;

      if (left) {
        if (top) {
          if (heightmap[0][0] < heightmap[1][0] && heightmap[0][0] < heightmap[0][1]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        } else if (bottom) {
          if (heightmap[i][0] < heightmap[i-1][0] && heightmap[i][0] < heightmap[i][1]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        } else {
          if (heightmap[i][0] < heightmap[i+1][0] && heightmap[i][0] < heightmap[i][1] && heightmap[i][0] < heightmap[i-1][0]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        }
      } else if (right) {
        if (top) {
          if (heightmap[0][j] < heightmap[1][j] && heightmap[0][j] < heightmap[0][j-1]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        } else if (bottom) {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        } else {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1] && heightmap[i][j] < heightmap[i+1][j]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        }        
      } else {
        if (top) {
          if (heightmap[0][j] < heightmap[0][j-1] && heightmap[0][j] < heightmap[0][j+1] && heightmap[0][j] < heightmap[1][j]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        } else if (bottom) {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1] && heightmap[i][j] < heightmap[i][j+1]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
        } else {
          if (heightmap[i][j] < heightmap[i-1][j] && heightmap[i][j] < heightmap[i][j-1] && heightmap[i][j] < heightmap[i][j+1] && heightmap[i][j] < heightmap[i+1][j]) {
            basins.push_back(count_to_9({}, heightmap, i, j));
          }
      }
    }
  }
  }

  std::sort(basins.begin(), basins.end(), std::greater<int>());
  
  std::cout << basins[0] * basins[1] * basins[2] << std::endl;
}