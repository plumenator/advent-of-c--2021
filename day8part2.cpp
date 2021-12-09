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

auto identity = [](auto s) {
    return s; 
};

template<typename T>
std::vector<T> split(std::string line, const std::string& delim, auto toT(std::string) -> T) {
  std::vector<T> out;
  for (;;) {
    if(auto delim_position = line.find(delim);
          delim_position != std::string::npos) {
      out.push_back(toT(line.substr(0, delim_position)));
      line = line.substr(delim_position + delim.size());
    } else {
    break;
    }
  }
  out.push_back(toT(line));
  return out;
}

template<typename T>
std::vector<T> split(std::string line, const std::string& delim) {
  return split<T>(line, delim, identity);
}

template<typename T>
std::vector<T> split(std::string line) {
  return split<T>(line, " ");
}

template<typename T>
std::set<T> diff(const std::set<T>& first_set, const std::set<T>& second_set) {
  std::set<T> out;
  std:set_difference(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), std::inserter(out, out.begin()));
  return out;
}

enum class Digit {
  Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine
};

enum class SegmentCount{
  Two = 2, Three, Four, Five, Six, Seven
};

using SegmentSet = std::set<char>;

int main() {     
  std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> lines;
  for (std::string line; std::getline(std::cin, line);) {
    auto parts = split<std::string>(line, " | ");
    assert(parts.size() == 2);
    auto unique_signals = split<std::string>(parts[0]);
    assert(unique_signals.size() == 10);
    auto number = split<std::string>(parts[1]);
    assert(number.size() == 4);
    lines.emplace_back(unique_signals, number);
  }

  const std::map<SegmentCount, Digit> unique_count = {
    {SegmentCount::Two, Digit::One},
    {SegmentCount::Four, Digit::Four},
    {SegmentCount::Three, Digit::Seven},
    {SegmentCount::Seven, Digit::Eight}
    };

  const std::map<Digit, SegmentSet> reference_sets = {
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
  for (auto& [unique_signals, number]: lines) {
    std::map<Digit, SegmentSet> actual_sets;
    std::vector<SegmentSet> six_sets;
    std::vector<SegmentSet> five_sets;

    for (auto& unique_signal: unique_signals) {
      auto current_count = SegmentCount(unique_signal.size());
      if (unique_count.contains(current_count)) {
        actual_sets[unique_count.at(current_count)] = SegmentSet(unique_signal.begin(), unique_signal.end());
      } else if (current_count == SegmentCount::Six) {
        six_sets.push_back(SegmentSet(unique_signal.begin(), unique_signal.end()));
      } else {
        five_sets.push_back(SegmentSet(unique_signal.begin(), unique_signal.end()));
      }
    }

    assert(actual_sets.size() == 4);
    assert(six_sets.size() == 3);
    assert(five_sets.size() == 3);

    for (auto& six_set: six_sets) {
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

    for (auto& five_set: five_sets) {
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

    assert(actual_sets.size() == 10);

    std::map<SegmentSet, Digit> decoder;
    for (auto [digit, segment_set]: actual_sets) {
      decoder.emplace(segment_set, digit);
    }

    for (int i = 0; i < number.size(); ++i) {
      auto number_str = number[i];
      auto segment_set = SegmentSet(number_str.begin(), number_str.end());
      sum += std::pow(10, number.size() - i - 1) * static_cast<int>(decoder.at(SegmentSet(number_str.begin(), number_str.end())));
    }
  }

  std::cout << sum << std::endl;
}