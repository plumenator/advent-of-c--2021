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

int count_to_9(std::set<std::pair<int, int>>&& visited, const std::vector<std::vector<int>>& heightmap, int i, int j) {
  if (visited.contains(std::pair<int, int>(i, j)))
    return 0;
  visited.insert(std::pair<int, int>(i,j));

  auto toright = [&]() {
     return heightmap[i][j + 1] < 9
            ? count_to_9(std::move(visited), heightmap, i, j + 1)
            : 0;
  };
  auto todown = [&]() {
    return heightmap[i + 1][j] < 9
           ? count_to_9(std::move(visited), heightmap, i + 1, j)
           : 0;
  };
  auto toup = [&]() {
    return heightmap[i - 1][j] < 9
           ? count_to_9(std::move(visited), heightmap, i - 1, j)
           : 0;
  };
  auto toleft = [&]() {
    return heightmap[i][j - 1] < 9
    ? count_to_9(std::move(visited), heightmap, i, j - 1)
    : 0;
  };

  bool top = i == 0;
  bool bottom = i == heightmap.size() - 1;
  bool left = j == 0;
  bool right = j == heightmap[j].size() - 1;

  auto topbottom = [&]() {
    return top
           ? todown()
           : bottom
           ? toup()
           : toup() + todown();
  };

  return 1 + (left
              ? toright() + topbottom()
              : right
              ? toleft() + topbottom()
              : toleft() + toright() + topbottom());
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
      auto toleft = [&]() {
        return heightmap[i][j] < heightmap[i][j - 1];
      };
      auto toright = [&]() {
        return heightmap[i][j] < heightmap[i][j + 1];
      };
      auto todown = [&]() {
        return heightmap[i][j] < heightmap[i + 1][j];
      };
      auto toup = [&]() {
        return heightmap[i][j] < heightmap[i - 1][j];
      };
      bool topdown = top
                     ? todown()
                     : bottom
                     ? toup()
                     : todown() && toup();
      bool leftright = left
                       ? toright()
                       : right
                       ? toleft()
                       : toright() && toleft();
      bool lowpoint = topdown && leftright;
      if (lowpoint)
        basins.push_back(count_to_9({}, heightmap, i, j));
    }
  }

  std::sort(basins.begin(), basins.end(), std::greater<int>());
  
  std::cout << basins[0] * basins[1] * basins[2] << std::endl;
}