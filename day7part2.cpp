/*
--- Part Two ---
The crabs don't seem interested in your proposed solution. Perhaps you misunderstand crab engineering?

As it turns out, crab submarine engines don't burn fuel at a constant rate. Instead, each change of 1 step in horizontal position costs 1 more unit of fuel than the last: the first step costs 1, the second step costs 2, the third step costs 3, and so on.

As each crab moves, moving further becomes more expensive. This changes the best horizontal position to align them all on; in the example above, this becomes 5:

Move from 16 to 5: 66 fuel
Move from 1 to 5: 10 fuel
Move from 2 to 5: 6 fuel
Move from 0 to 5: 15 fuel
Move from 4 to 5: 1 fuel
Move from 2 to 5: 6 fuel
Move from 7 to 5: 3 fuel
Move from 1 to 5: 10 fuel
Move from 2 to 5: 6 fuel
Move from 14 to 5: 45 fuel
This costs a total of 168 fuel. This is the new cheapest possible outcome; the old alignment position (2) now costs 206 fuel instead.

Determine the horizontal position that the crabs can align to using the least fuel possible so they can make you an escape route! How much fuel must they spend to align to that position?
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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
  // std::cout << line << "\n";
  auto positions = split_ints(line, ",");
  // std::cout << positions[0] << "..." << positions.back() << std::endl;

  auto [min, max] = std::minmax_element(positions.begin(), positions.end());
  // std::cout << "Min:" << *min << std::endl;
  // std::cout << "Max:" << *max << std::endl;
  
  std::vector<int> fuels;
  for (int i = *min; i <= *max; ++i) {
    int total = 0;
    for (auto position: positions) {
      int step = std::abs(position - i);
      total += step * (step + 1) / 2;
    }
    fuels.push_back(total);
  }
  
  auto min_fuel = std::min_element(fuels.begin(), fuels.end());

  std::cout << *min_fuel << std::endl;
}