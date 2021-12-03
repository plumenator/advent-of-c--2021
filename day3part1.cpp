/*
--- Day 3: Binary Diagnostic ---
The submarine has been making some odd creaking noises, so you ask it to produce a diagnostic report just in case.

The diagnostic report (your puzzle input) consists of a list of binary numbers which, when decoded properly, can tell you many useful things about the conditions of the submarine. The first parameter to check is the power consumption.

You need to use the binary numbers in the diagnostic report to generate two new binary numbers (called the gamma rate and the epsilon rate). The power consumption can then be found by multiplying the gamma rate by the epsilon rate.

Each bit in the gamma rate can be determined by finding the most common bit in the corresponding position of all numbers in the diagnostic report. For example, given the following diagnostic report:

00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010
Considering only the first bit of each number, there are five 0 bits and seven 1 bits. Since the most common bit is 1, the first bit of the gamma rate is 1.

The most common second bit of the numbers in the diagnostic report is 0, so the second bit of the gamma rate is 0.

The most common value of the third, fourth, and fifth bits are 1, 1, and 0, respectively, and so the final three bits of the gamma rate are 110.

So, the gamma rate is the binary number 10110, or 22 in decimal.

The epsilon rate is calculated in a similar way; rather than use the most common bit, the least common bit from each position is used. So, the epsilon rate is 01001, or 9 in decimal. Multiplying the gamma rate (22) by the epsilon rate (9) produces the power consumption, 198.

Use the binary numbers in your diagnostic report to calculate the gamma rate and epsilon rate, then multiply them together. What is the power consumption of the submarine? (Be sure to represent your answer in decimal, not binary.)
*/

#include <iostream>
#include <string>
#include <array>
#include <cmath>

constexpr size_t size = 12;

int from_ones(const std::array<int, size>& binary)
{
  int num = 0;
  int index = 0;
  for (auto i = binary.rbegin(); i != binary.rend(); ++i, ++index) {
    num = num + *i * std::pow(2, index);
  }
  std::cout << "Number:" << num << std::endl;
  return num;
}

std::array<int, size> to_binary(const std::array<int, size>& ones, int total) {
  std::array<int, size> binary;
  auto one = ones.begin();
  auto b = binary.begin();
  std::cout << "Binary:";
  for (; one != ones.end(); ++one, ++b) {
    *b = *one > total/2 ? 1 : 0;
    std::cout << *b;
  }
  std::cout << std::endl;
  return binary;
}

int main() {
  std::array<int, size> ones {0, 0, 0, 0, 0};
  int total = 0;
  for (std::string line; std::getline(std::cin, line);) {
    ++total;
    auto one = ones.begin();
    auto c = line.begin();
    for (; one != ones.end(); ++one, ++c) {
      *one += *c - '0';
    }
  }
  auto gamma_bits = to_binary(ones, total);
  auto gamma = from_ones(gamma_bits);

  std::array<int, size> epsilon_bits;
  auto gamma_bit = gamma_bits.begin();
  auto epsilon_bit = epsilon_bits.begin();
  for (; gamma_bit != gamma_bits.end(); ++gamma_bit, ++epsilon_bit) {
    *epsilon_bit = *gamma_bit == 1 ? 0 : 1;
  }
  auto epsilon = from_ones(epsilon_bits);

  std::cout << gamma * epsilon << std::endl;
}
