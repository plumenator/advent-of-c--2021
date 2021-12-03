/*
--- Part Two ---
Next, you should verify the life support rating, which can be determined by multiplying the oxygen generator rating by the CO2 scrubber rating.

Both the oxygen generator rating and the CO2 scrubber rating are values that can be found in your diagnostic report - finding them is the tricky part. Both values are located using a similar process that involves filtering out values until only one remains. Before searching for either rating value, start with the full list of binary numbers from your diagnostic report and consider just the first bit of those numbers. Then:

Keep only numbers selected by the bit criteria for the type of rating value for which you are searching. Discard numbers which do not match the bit criteria.
If you only have one number left, stop; this is the rating value for which you are searching.
Otherwise, repeat the process, considering the next bit to the right.
The bit criteria depends on which type of rating value you want to find:

To find oxygen generator rating, determine the most common value (0 or 1) in the current bit position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values with a 1 in the position being considered.
To find CO2 scrubber rating, determine the least common value (0 or 1) in the current bit position, and keep only numbers with that bit in that position. If 0 and 1 are equally common, keep values with a 0 in the position being considered.
For example, to determine the oxygen generator rating value using the same example diagnostic report from above:

Start with all 12 numbers and consider only the first bit of each number. There are more 1 bits (7) than 0 bits (5), so keep only the 7 numbers with a 1 in the first position: 11110, 10110, 10111, 10101, 11100, 10000, and 11001.
Then, consider the second bit of the 7 remaining numbers: there are more 0 bits (4) than 1 bits (3), so keep only the 4 numbers with a 0 in the second position: 10110, 10111, 10101, and 10000.
In the third position, three of the four numbers have a 1, so keep those three: 10110, 10111, and 10101.
In the fourth position, two of the three numbers have a 1, so keep those two: 10110 and 10111.
In the fifth position, there are an equal number of 0 bits and 1 bits (one each). So, to find the oxygen generator rating, keep the number with a 1 in that position: 10111.
As there is only one number left, stop; the oxygen generator rating is 10111, or 23 in decimal.
Then, to determine the CO2 scrubber rating value from the same example above:

Start again with all 12 numbers and consider only the first bit of each number. There are fewer 0 bits (5) than 1 bits (7), so keep only the 5 numbers with a 0 in the first position: 00100, 01111, 00111, 00010, and 01010.
Then, consider the second bit of the 5 remaining numbers: there are fewer 1 bits (2) than 0 bits (3), so keep only the 2 numbers with a 1 in the second position: 01111 and 01010.
In the third position, there are an equal number of 0 bits and 1 bits (one each). So, to find the CO2 scrubber rating, keep the number with a 0 in that position: 01010.
As there is only one number left, stop; the CO2 scrubber rating is 01010, or 10 in decimal.
Finally, to find the life support rating, multiply the oxygen generator rating (23) by the CO2 scrubber rating (10) to get 230.

Use the binary numbers in your diagnostic report to calculate the oxygen generator rating and CO2 scrubber rating, then multiply them together. What is the life support rating of the submarine? (Be sure to represent your answer in decimal, not binary.)
*/

#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include <cmath>
#include <vector>

int to_decimal(const std::string& binary)
{
  // std::cout << "Binary:" << binary << std::endl;
  int num = 0;
  int index = 0;
  for (auto i = binary.rbegin(); i != binary.rend(); ++i, ++index) {
    num = num + (*i - '0') * std::pow(2, index);
  }
  // std::cout << "Number:" << num << std::endl;
  return num;
}
enum MaxBit {
  Zero,
  One,
  Both
};

std::vector<MaxBit> to_binary(const std::vector<int>& ones, int total) {
  std::vector<MaxBit> binary(ones.size());
  auto one = ones.begin();
  auto b = binary.begin();
  // std::cout << "Maxes:";
  for (; one != ones.end(); ++one, ++b) {
    *b = 2 * *one > total ? One :
         2 * *one == total ?  Both :
         Zero;
    std::cout << *b;
  }
  std::cout << std::endl;
  return binary;
}

std::vector<std::string> filter(const std::vector<std::string>& original, int index, int bit, int prefer) {
  // std::cout << "filter(" << index << "," << bit << "," << prefer << "):";
  std::vector<std::string> out;
  for (auto o: original) {
    int actual = bit;
    if(actual == Both) {
      actual = prefer;
    }
    if (o[index] - '0' == actual) {
      out.push_back(o);
     //  std::cout << o << ";";
    }
  }
  // std::cout << std::endl;
  return out;
}

std::vector<MaxBit> gen_gamma(const std::vector<std::string>& lines) {
  int total = lines.size();
  std::vector<int> ones(lines[0].size());
  // std::cout << "gamma:";
  for (auto& line: lines) {
    // std::cout << line << ";";
    auto one = ones.begin();
    auto c = line.begin();
    for (; c != line.end(); ++one, ++c) {
      *one += *c - '0';
    }
  }
  // std::cout << std::endl;
  return to_binary(ones, total);
}

std::vector<MaxBit> gen_epsilon(const std::vector<std::string>& lines) {
  auto gamma_bits = gen_gamma(lines);
  std::vector<MaxBit> epsilon_bits(gamma_bits.size());
  auto gamma_bit = gamma_bits.begin();
  auto epsilon_bit = epsilon_bits.begin();
  for (; gamma_bit != gamma_bits.end(); ++gamma_bit, ++epsilon_bit) {
    *epsilon_bit = *gamma_bit == One ? Zero :
                   *gamma_bit == Zero ? One :
                   Both;
  }
  return epsilon_bits;
}

int main() {
  std::vector<std::string> lines;
  for (std::string line; std::getline(std::cin, line); )
    lines.push_back(line);
  std::vector<std::string> oxygen = lines;
  for (int i = 0; i < lines[0].size() && oxygen.size() != 1; ++i) {
    auto gamma_bits = gen_gamma(oxygen);
    oxygen = filter(oxygen, i, gamma_bits[i], 1);
  }
  assert(oxygen.size() == 1);
  std::vector<std::string> scrubber = lines;
  for (int i = 0; i < lines[0].size() && scrubber.size() != 1; ++i) {
    auto epsilon_bits = gen_epsilon(scrubber);
    scrubber = filter(scrubber, i, epsilon_bits[i], 0);
  }
  assert(scrubber.size() == 1);
  std::cout << to_decimal(oxygen[0]) * to_decimal(scrubber[0]) << std::endl;
}