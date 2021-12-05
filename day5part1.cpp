/*
--- Day 5: Hydrothermal Venture ---
You come across a field of hydrothermal vents on the ocean floor! These vents constantly produce large, opaque clouds, so it would be best to avoid them if possible.

They tend to form in lines; the submarine helpfully produces a list of nearby lines of vents (your puzzle input) for you to review. For example:

0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2
Each line of vents is given as a line segment in the format x1,y1 -> x2,y2 where x1,y1 are the coordinates of one end the line segment and x2,y2 are the coordinates of the other end. These line segments include the points at both ends. In other words:

An entry like 1,1 -> 1,3 covers points 1,1, 1,2, and 1,3.
An entry like 9,7 -> 7,7 covers points 9,7, 8,7, and 7,7.
For now, only consider horizontal and vertical lines: lines where either x1 = x2 or y1 = y2.

So, the horizontal and vertical lines from the above list would produce the following diagram:

.......1..
..1....1..
..1....1..
.......1..
.112111211
..........
..........
..........
..........
222111....
In this diagram, the top left corner is 0,0 and the bottom right corner is 9,9. Each position is shown as the number of lines which cover that point or . if no line covers that point. The top-left pair of 1s, for example, comes from 2,2 -> 2,1; the very bottom row is formed by the overlapping lines 0,9 -> 5,9 and 0,9 -> 2,9.

To avoid the most dangerous areas, you need to determine the number of points where at least two lines overlap. In the above example, this is anywhere in the diagram with a 2 or larger - a total of 5 points.

Consider only horizontal and vertical lines. At how many points do at least two lines overlap?
*/

#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include <cmath>
#include <vector>

std::pair<std::string, std::string> split(const std::string& text, std::string delim) {
  auto delim_begin = text.find(delim);
  auto delim_end = delim_begin + delim.size();
  return {text.substr(0, delim_begin), text.substr(delim_end)};
}

using Point = std::pair<int, int>;
using Line = std::pair<Point, Point>;

Line parse_line(std::string line) {
  // std::cout << line << ": ";
  auto [begin, end] = split(line, "->");
  auto [x1, y1] = split(begin, ",");
  auto [x2, y2] = split(end, ",");
  // std::cout << x1 << "," << y1 << "->" << x2 << "," << y2 << std::endl;
  return {{std::stoi(x1), std::stoi(y1)}, {std::stoi(x2), std::stoi(y2)}};
}


int main() {
  std::vector<Line> lines;
  for (std::string line; std::getline(std::cin, line);) {
    lines.push_back(parse_line(line));
  }

  int grid[990][990] = {0};
  int total = 0;
  for (int i = 0; i < 990; ++i) {
    for (int j = 0; j < 990; ++j) {
      for (auto& line: lines) {
        auto& x1 = line.first.first;
        auto& y1 = line.first.second;
        auto& x2 = line.second.first;
        auto& y2 = line.second.second;
        if (x1 == x2) {
          // vertical line
          if (i == x1) {
            auto min = std::min(y1, y2);
            auto max = std::max(y1, y2);
            if (j >= min && j <= max) {
            ++grid[i][j];
            }
          }
        }
        if (y1 == y2) {
          // horizontal line
          if (j == y1) {
            auto min = std::min(x1, x2);
            auto max = std::max(x1, x2);
            if (i >= min && i <= max) {
            ++grid[i][j];
            }
          }
        }
      // std::cout << x1 << "," << y1 << "->" << x2 << "," << y2 << ": ";
      // std::cout << i << "," << j << " :" << grid[i][j] << std::endl;
      }
      // std::cout << grid[i][j] << " ";
      if (grid[i][j] >= 2) {
        ++total;
      }
    }
    // std::cout << std::endl;
  }
  std::cout << total << std::endl;
}