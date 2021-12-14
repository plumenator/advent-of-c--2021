/*
--- Day 14: Extended Polymerization ---
The incredible pressures at this depth are starting to put a strain on your submarine. The submarine has polymerization equipment that would produce suitable materials to reinforce the submarine, and the nearby volcanically-active caves should even have the necessary input elements in sufficient quantities.

The submarine manual contains instructions for finding the optimal polymer formula; specifically, it offers a polymer template and a list of pair insertion rules (your puzzle input). You just need to work out what polymer would result after repeating the pair insertion process a few times.

For example:

NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C
The first line is the polymer template - this is the starting point of the process.

The following section defines the pair insertion rules. A rule like AB -> C means that when elements A and B are immediately adjacent, element C should be inserted between them. These insertions all happen simultaneously.

So, starting with the polymer template NNCB, the first step simultaneously considers all three pairs:

The first pair (NN) matches the rule NN -> C, so element C is inserted between the first N and the second N.
The second pair (NC) matches the rule NC -> B, so element B is inserted between the N and the C.
The third pair (CB) matches the rule CB -> H, so element H is inserted between the C and the B.
Note that these pairs overlap: the second element of one pair is the first element of the next pair. Also, because all pairs are considered simultaneously, inserted elements are not considered to be part of a pair until the next step.

After the first step of this process, the polymer becomes NCNBCHB.

Here are the results of a few steps using the above rules:

Template:     NNCB
After step 1: NCNBCHB
After step 2: NBCCNBBBCBHCB
After step 3: NBBBCNCCNBBNBNBBCHBHHBCHB
After step 4: NBBNBNBBCCNBCNCCNBBNBBNBBBNBBNBBCBHCBHHNHCBBCBHCB
This polymer grows quickly. After step 5, it has length 97; After step 10, it has length 3073. After step 10, B occurs 1749 times, C occurs 298 times, H occurs 191 times, and N occurs 865 times; taking the quantity of the most common element (B, 1749) and subtracting the quantity of the least common element (H, 161) produces 1749 - 161 = 1588.

Apply 10 steps of pair insertion to the polymer template and find the most and least common elements in the result. What do you get if you take the quantity of the most common element and subtract the quantity of the least common element?
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
#include <cctype>
#include <list>

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

std::string insert(std::string&& polymer_template, const std::map<std::pair<char, char>, char>& rules) {
  std::string out;
  for(auto f = polymer_template.begin(), s = polymer_template.begin() + 1;
      f != polymer_template.end();
      ++f, ++s
      ) {
        auto loc = rules.find({*f, *s});
        if (loc == rules.end()) {
          out += *f;
        } else {
          out += *f;
          out += loc->second;
        }
  }
  return out;
}

int main() {     
  std::string polymer_template;
  std::getline(std::cin, polymer_template);
  {
    std::string line;
    std::getline(std::cin, line);
    assert(line.empty());
  }
  std::map<std::pair<char, char>, char> rules;
  for (std::string line; std::getline(std::cin, line);) {
    auto rule_strs = split<std::string>(line, " -> ");
    assert(rule_strs.size() == 2);
    assert(rule_strs[0].size() == 2);
    assert(rule_strs[1].size() == 1);
    rules.emplace(std::pair<char, char>{rule_strs[0][0], rule_strs[0][1]}, rule_strs[1][0]);
  }
  constexpr int count = 10;
  for (int i = 0; i < count; ++i) {
    polymer_template = insert(std::move(polymer_template), rules);
    std::cout << i + 1 << ": " << polymer_template.size() << std::endl;
  }
  
  std::multiset<char> counts(polymer_template.begin(), polymer_template.end());
  auto [min, max] = std::minmax_element(std::begin(counts), std::end(counts), [&](const char& a, const char& b) {
    return counts.count(a) < counts.count(b);
  });
  std::cout << counts.count(*max) - counts.count(*min) << std::endl;
}