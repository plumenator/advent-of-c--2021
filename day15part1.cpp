/*
--- Day 15: Chiton ---
You've almost reached the exit of the Risk, but the walls are getting closer together. Your submarine can barely still fit, though; the main problem is that the walls of the Risk are covered in chitons, and it would be best not to bump any of them.

The Riskrn is large, but has a very low ceiling, restricting your motion to two dimensions. The shape of the Riskrn resembles a square; a quick scan of chiton density produces a map of risk level throughout the Risk (your puzzle input). For example:

1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581
You start in the top left position, your destination is the bottom right position, and you cannot move diagonally. The number at each position is its risk level; to determine the total risk of an entire path, add up the risk levels of each position you enter (that is, don't count the risk level of your starting position unless you enter it; leaving it adds no risk to your total).

Your goal is to find a path with the lowest total risk. In this example, a path with the lowest total risk is highlighted here:

1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581
The total risk of this path is 40 (the starting position is never entered, so its risk is not counted).

What is the lowest total risk of any path from the top left to the bottom right?
*/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
#include <array>
#include <cassert>
#include <cmath>
#include <cctype>
#include <list>
#include <limits>

using Risk = unsigned long long;
using Grid = std::vector<std::vector<Risk>>;
constexpr Risk nope = std::numeric_limits<Risk>::max() - 9;

Risk lowest_risk(const Grid& grid, int i, int j, int isize, int jsize, std::set<std::pair<int, int>>& visited, std::map<std::pair<int, int>, Risk>& memo) {
  // std::cout << i << ", " << j << std::endl;
  bool top = i == 0;
  bool bottom = i == isize - 1;
  bool left = j == 0;
  bool right = j == jsize - 1;
  if (bottom && right) {
    // std::cout << "found a path" << std::endl;
    return 0;
  }
  if (visited.contains(std::make_pair(i, j))) {
    // std::cout << "nope" << std::endl;
    return nope;
  }
  visited.insert(std::make_pair(i, j));
  auto toleft = [&]() {
    return grid[i][j - 1] + lowest_risk(grid, i, j - 1, isize, jsize, visited, memo);
  };
  auto toright = [&]() {
    return grid[i][j + 1] + lowest_risk(grid, i, j + 1, isize, jsize, visited, memo);
  };
  auto todown = [&]() {
    return grid[i + 1][j] + lowest_risk(grid, i + 1, j, isize, jsize, visited, memo);
  };
  auto toup = [&]() {
    return grid[i - 1][j] + lowest_risk(grid, i - 1, j, isize, jsize, visited, memo);
  };
  auto topdown = [&]() {
    return top
  ? todown()
  : bottom
  ? nope // toup()
  : todown(); // std::min(todown() , toup());
  };
  auto leftright = [&]() {
    return left
  ? toright()
  : right
  ? nope // toleft()
  : toright(); //std::min(toright() , toleft());
  };
  visited.erase(std::make_pair(i, j));
  auto memoized = memo.find(std::make_pair(i, j));
  auto lowest = memoized != memo.end() 
                ? memoized->second
                : std::min(topdown(), leftright());
  memo.emplace(std::make_pair(i, j), lowest);
  return lowest;
}

int main() {
  Grid grid;
  for (std::string line; std::getline(std::cin, line);) {
    grid.push_back({});
    for (auto risk: line) {
      grid.back().push_back(risk - '0');
    }
  }

  std::set<std::pair<int, int>> visited;
  std::map<std::pair<int, int>, Risk> memo;
  std::cout << lowest_risk(grid, 0, 0, grid.size(), grid[0].size(), visited, memo) << std::endl;
}
