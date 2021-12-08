/*
--- Part Two ---
Through a little deduction, you should now be able to determine the remaining digit. Consider again the first example above:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf
After some careful analysis, the mapping between signal wires and segmentCount only make sense in the following configuration:

 dddd
e    a
e    a
 ffff
g    b
g    b
 cccc
So, the unique signal patterns would correspond to the following digit:

acedgfb: 8
cdfbe: 5
gcdfa: 2
fbcad: 3
dab: 7
cefabd: 9
cdfgeb: 6
eafb: 4
cagedb: 0
ab: 1
Then, the four digit of the output value can be decoded:

cdfeb: 5
fcadb: 3
cdfeb: 5
cdbaf: 3
Therefore, the output value for this entry is 5353.

Following this same process for each entry in the second, larger example above, the output value of each entry can be determined:

fdgacbe cefdb cefbgd gcbe: 8394
fcgedb cgb dgebacf gc: 9781
cg cg fdcagb cbg: 1197
efabcd cedba gadfec cb: 9361
gecf egdcabf bgf bfgea: 4873
gebdcfa ecba ca fadegcb: 8418
cefg dcbef fcge gbcadfe: 4548
ed bcgafe cdgba cbgef: 1625
gbdfcae bgc cg cgb: 8717
fgae cfgab fg bagce: 4315
Adding all of the output values in this larger example produces 61229.

For each entry, determine all of the wire/segment connections and decode the four-digit output values. What do you get if you add up all of the output values?
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

template<typename T>
std::vector<T> split(std::string line, std::string delim, auto toT(const std::string&) -> T) {
  std::vector<T> out;
  for (;;) {
    if(auto delim_position = line.find(delim);
          delim_position != std::string::npos) {
      auto num_str = line.substr(0, delim_position);
      out.push_back(toT(num_str));
      line = line.substr(delim_position + delim.size());
    } else {
    break;
    }
  }
  line = line[0] == ' ' ? line.substr(1) : line;
  out.push_back(toT(line));
  return out;
}

enum class Digit {
  Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine
};

enum class SegmentCount{
  Two = 2, Three, Four, Five, Six, Seven
};

std::set<char> diff(const std::map<Digit, std::set<char>>& actual_sets, Digit first, Digit second) {
  auto& first_set = actual_sets.at(first);
  auto& second_set = actual_sets.at(second);
  std::set<char> out;
  std:set_difference(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), std::inserter(out, out.begin()));
  return out;
}

std::set<char> diff(const std::set<char>& first_set, const std::set<char>& second_set) {
  std::set<char> out;
  std:set_difference(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), std::inserter(out, out.begin()));
  return out;
}

int main() {
  auto identity = [](const std::string& s){
     return std::string(s); 
     };
     
  std::vector<std::pair<std::array<std::string, 10>, std::array<std::string, 4>>> lines;
  for (std::string line; std::getline(std::cin, line);) {
    auto parts = split<std::string>(line, " | ", identity);
    std::array<std::string, 10> unique_signals;
    auto unique_signals_str = split<std::string>(parts[0], " ", identity);
    std::copy_n(std::make_move_iterator(unique_signals_str.begin()), 10, unique_signals.begin());
    std::array<std::string, 4> number;
    auto number_strs = split<std::string>(parts[1], " ", identity);
    std::copy_n(std::make_move_iterator(number_strs.begin()), 4, number.begin());
    lines.emplace_back(unique_signals, number);
  }
  const std::map<SegmentCount, Digit> unique_count = {
    {SegmentCount::Two, Digit::One},
    {SegmentCount::Four, Digit::Four},
    {SegmentCount::Three, Digit::Seven},
    {SegmentCount::Seven, Digit::Eight}
    };

  const std::map<Digit, std::set<char>> reference_sets = {
    {Digit::Zero, {'a', 'b', 'c', 'e', 'f', 'g'}},
    {Digit::One, {'c', 'f'}},
    {Digit::Two, {'a', 'd', 'c', 'e', 'g'}},
    {Digit::Three, {'a', 'd', 'c', 'f', 'g'}},
    {Digit::Four, {'b', 'd', 'c', 'f'}},
    {Digit::Five, {'a', 'd', 'b', 'f', 'g'}},
    {Digit::Six, {'a', 'd', 'b', 'e', 'g', 'f'}},
    {Digit::Seven, {'a', 'c', 'f'}},
    {Digit::Eight, {'a', 'b', 'c', 'd', 'e', 'f', 'g'}},
    {Digit::Nine, {'a', 'b', 'c', 'd', 'f', 'g'}},
  };
  int sum = 0;
  for (auto [unique_signals, number]: lines) {
    std::map<Digit, std::set<char>> actual_sets;
    std::vector<std::set<char>> six_sets;
    std::vector<std::set<char>> five_sets;

    for (auto unique_signal: unique_signals) {
      SegmentCount current_count = static_cast<SegmentCount>(unique_signal.size());
      if (unique_count.contains(current_count)) {
        actual_sets[unique_count.at(current_count)] = std::set<char>(unique_signal.begin(), unique_signal.end());
      } else if (current_count == SegmentCount::Six) {
        six_sets.push_back(std::set<char>(unique_signal.begin(), unique_signal.end()));
      } else {
        five_sets.push_back(std::set<char>(unique_signal.begin(), unique_signal.end()));
      }
    }

    assert(actual_sets.size() == 4);
    assert(six_sets.size() == 3);
    assert(five_sets.size() == 3);

    for (auto six_set: six_sets) {
      auto c_or_d_or_e = diff(actual_sets.at(Digit::Eight), six_set);
      assert(c_or_d_or_e.size() == 1);
      if (diff(c_or_d_or_e, actual_sets.at(Digit::One)).size() == 1) {
        auto d_or_e = c_or_d_or_e;
        if (diff(d_or_e, actual_sets.at(Digit::Four)).size() == 1) {
          actual_sets[Digit::Nine] = six_set;
        } else {
          actual_sets[Digit::Zero] = six_set;
        }
      } else {
        actual_sets[Digit::Six] = six_set;
      }
    }

    for (auto five_set: five_sets) {
      auto e_or_none = diff(five_set, actual_sets.at(Digit::Nine));
      assert(e_or_none.size() < 2);
      if (e_or_none.size() == 1) {
        actual_sets[Digit::Two] = five_set;
      } else {
        auto c_or_none = diff(five_set, actual_sets.at(Digit::Six));
        assert(c_or_none.size() < 2);
        if (c_or_none.size() == 1) {
            actual_sets[Digit::Three] = five_set;
          } else {
            actual_sets[Digit::Five] = five_set;
          }
      }
    }

    std::map<std::set<char>, Digit> decoder;
    for (auto [digit, segment_set]: actual_sets) {
      decoder.emplace(segment_set, digit);
    }

    for (int i = 0; i < number.size(); ++i) {
      auto number_str = number[i];
      std::set<char> segment_set = std::set<char>(number_str.begin(), number_str.end());
      sum += std::pow(10, number.size() - i - 1) * static_cast<int>(decoder.at(std::set<char>(number_str.begin(), number_str.end())));
    }
  }

  std::cout << sum << std::endl;
}