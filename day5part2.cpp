/*
--- Part Two ---
Unfortunately, considering only horizontal and vertical lines doesn't give you the full picture; you need to also consider diagonal lines.

Because of the limits of the hydrothermal vent mapping system, the lines in your list will only ever be horizontal, vertical, or a diagonal line at exactly 45 degrees. In other words:

An entry like 1,1 -> 3,3 covers points 1,1, 2,2, and 3,3.
An entry like 9,7 -> 7,9 covers points 9,7, 8,8, and 7,9.
Considering all lines from the above example would now produce the following diagram:

1.1....11.
.111...2..
..2.1.111.
...1.2.2..
.112313211
...1.2....
..1...1...
.1.....1..
1.......1.
222111....
You still need to determine the number of points where at least two lines overlap. In the above example, this is still anywhere in the diagram with a 2 or larger - now a total of 12 points.

Consider all of the lines. At how many points do at least two lines overlap?
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
  constexpr size_t size = 990;
  int grid[size][size] = {0};
  int total = 0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      for (auto& line: lines) {
        auto& x1 = line.first.first;
        auto& y1 = line.first.second;
        auto& x2 = line.second.first;
        auto& y2 = line.second.second;
        if ((x2 - x1) * (j - y1) == (i - x1) * (y2 - y1)){
          if (x1 == x2) {
            if ((y1 <= j && j <= y2) || 
                (y2 <= j && j <= y1)) {
                  ++grid[i][j];
                }
          } else {
            if ((x1 <= i && i <= x2) || 
                (x2 <= i && i <= x1)) {
                  ++grid[i][j];
                }
          }
        }
      }
      if (grid[i][j] >= 2) {
        ++total;
      }
    }
  }
  std::cout << total << std::endl;
}