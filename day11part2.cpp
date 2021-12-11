/*
--- Part Two ---
It seems like the individual flashes aren't bright enough to navigate. However, you might have a better option: the flashes seem to be synchronizing!

In the example above, the first time all octopuses flash simultaneously is step 195:

After step 193:
5877777777
8877777777
7777777777
7777777777
7777777777
7777777777
7777777777
7777777777
7777777777
7777777777

After step 194:
6988888888
9988888888
8888888888
8888888888
8888888888
8888888888
8888888888
8888888888
8888888888
8888888888

After step 195:
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
0000000000
If you can calculate the exact moments when the octopuses will all flash simultaneously, you should be able to navigate through the cavern. What is the first step during which all octopuses flash?
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

void bump(std::vector<std::vector<int>>& energies) {
  for (auto& row: energies) {
    for (auto& energy: row) {
      ++energy;
    }
  }
}

int propagate(std::set<std::pair<int, int>>& flashed, std::vector<std::vector<int>>& energies, int i, int j) {
  if (flashed.contains(std::pair<int, int>(i, j))
    || i < 0 || j < 0 || i >= energies.size() || j >= energies[0].size()
    )
    return 0;
  flashed.emplace(i,j);

  energies[i][j] = 0;

  auto toleft = [&]() {
    return !flashed.contains({i, j - 1}) && ++energies[i][j - 1] > 9
    ? propagate(flashed, energies, i, j - 1)
    : 0;
  };
  auto toright = [&]() {
     return !flashed.contains({i, j + 1}) && ++energies[i][j + 1] > 9
            ? propagate(flashed, energies, i, j + 1)
            : 0;
  };
  auto todown = [&]() {
    return !flashed.contains({i + 1, j}) && ++energies[i + 1][j] > 9
           ? propagate(flashed, energies, i + 1, j)
           : 0;
  };
  auto toup = [&]() {
    return !flashed.contains({i - 1, j}) && ++energies[i - 1][j] > 9
           ? propagate(flashed, energies, i - 1, j)
           : 0;
  };
  auto toleftup = [&]() {
    return !flashed.contains({i - 1, j - 1}) && ++energies[i - 1][j - 1] > 9
    ? propagate(flashed, energies, i - 1, j - 1)
    : 0;
  };
  auto torightup = [&]() {
     return !flashed.contains({i - 1, j + 1}) && ++energies[i - 1][j + 1] > 9
            ? propagate(flashed, energies, i - 1, j + 1)
            : 0;
  };
  auto torightdown = [&]() {
    return !flashed.contains({i + 1, j + 1}) && ++energies[i + 1][j + 1] > 9
           ? propagate(flashed, energies, i + 1, j + 1)
           : 0;
  };
  auto toleftdown = [&]() {
    return !flashed.contains({i + 1, j - 1}) && ++energies[i + 1][j - 1] > 9
           ? propagate(flashed, energies, i + 1, j - 1)
           : 0;
  };

  bool top = i == 0;
  bool bottom = i == energies.size() - 1;
  bool left = j == 0;
  bool right = j == energies[j].size() - 1;

  auto topbottom = [&]() {
    return top
           ? todown()
           : bottom
           ? toup()
           : toup() + todown();
  };

  auto diagonal = [&]() {
    return top
           ? (left
              ? torightdown()
              : right
              ? toleftdown()
              : torightdown() + toleftdown())
           : bottom
           ? (left
              ? torightup()
              : right
              ? toleftup()
              : torightup() + toleftup())
           : (left
              ? torightup() + torightdown()
              : right
              ? toleftup() + toleftdown()
              : torightdown() + toleftdown() + torightup() + toleftup());
  };

  return 1 + (left
              ? toright()
              : right
              ? toleft()
              : toleft() + toright()) + topbottom() + diagonal();
}

int flash(std::vector<std::vector<int>>& energies) {
  int flashes = 0;
  std::set<std::pair<int, int>> flashed;
  for (int i = 0; i < energies.size(); ++i) {
    for (int j = 0; j < energies[i].size(); ++j) {
      if (energies[i][j] > 9) {
        flashes += propagate(flashed, energies, i, j);
      }
    }
  }
  return flashes;
}

void print(const std::vector<std::vector<int>>& grid) {
  for (auto& row: grid) {
    for (auto& cell: row) {
      std::cout << cell;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

bool allflash(const std::vector<std::vector<int>>& grid) {
  for (auto& row: grid) {
    for (auto& cell: row) {
      if (cell != 0)
        return false;
    }
  }
  return true;
}

int main() {     
  std::vector<std::vector<int>> energies;
  for (std::string line; std::getline(std::cin, line);) {
    energies.push_back({});
    for (auto energy: line) {
      energies.back().push_back(energy - '0');
    }
  }
  int flashes = 0;
  for (int i = 0; true; ++i) {
    bump(energies);
    flashes += flash(energies);
    if (allflash(energies)) {
      std::cout << i + 1 << std::endl;
      break;
    }
  }
}