/*
--- Part Two ---
Suppose the lanternfish live forever and have unlimited food and space. Would they take over the entire ocean?

After 256 days in the example above, there would be a total of 26984457539 lanternfish!

How many lanternfish would there be after 256 days?
*/

#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include <cmath>
#include <vector>
#include <numeric>

std::vector<int> split_ints(std::string line, std::string delim) {
  std::vector<int> out;
  for (;;) {
    line = line[0] == ' ' ? line.substr(1) : line;
    if(auto delim_position = line.find_first_of(delim);
          delim_position != std::string::npos) {
      auto num_str = line.substr(0, delim_position);
      out.push_back(std::stoi(num_str));
      line = line.substr(delim_position + 1);
    } else {
    break;
    }
  }
  line = line[0] == ' ' ? line.substr(1) : line;
  out.push_back(std::stoi(line));
  return out;
}


int main() {
  std::string line;
  std::getline(std::cin, line);
  std::vector<int> fish_ages = split_ints(line, ",");
  std::array<long long, 9> fish_groups{0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (auto fish_age: fish_ages) {
    ++fish_groups[fish_age];
  }
  constexpr int days = 256;
  for (int day = 0; day < days; ++day) {
    auto new_fish = fish_groups[0];
    fish_groups[0] = fish_groups[1];
    fish_groups[1] = fish_groups[2];
    fish_groups[2] = fish_groups[3];
    fish_groups[3] = fish_groups[4];
    fish_groups[4] = fish_groups[5];
    fish_groups[5] = fish_groups[6];
    fish_groups[6] = new_fish + fish_groups[7];
    fish_groups[7] = fish_groups[8];
    fish_groups[8] = new_fish;
  }
  std::cout << std::accumulate(fish_groups.begin(), fish_groups.end(), 0ll) << std::endl;
}