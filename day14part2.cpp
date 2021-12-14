/*
--- Part Two ---
The resulting polymer isn't nearly strong enough to reinforce the submarine. You'll need to run more steps of the pair insertion process; a total of 40 steps should do it.

In the above example, the most common element is B (occurring 2192039569602 times) and the least common element is H (occurring 3849876073 times); subtracting these produces 2188189693529.

Apply 40 steps of pair insertion to the polymer template and find the most and least common elements in the result. What do you get if you take the quantity of the most common element and subtract the quantity of the least common element?
*/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <numeric>
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

using bignum = long long unsigned;

template<typename M, typename K>
void increment(M& m, K&& k, bignum inc = 1) {
  auto [it, inserted] = m.try_emplace(std::forward<K>(k), inc);
  if (!inserted) {
    it->second += inc;
  }
}

std::map<std::pair<char, char>, bignum> insert(std::map<std::pair<char, char>, bignum>&& polymer_template, std::map<char, bignum>& counts, const std::map<std::pair<char, char>, char>& rules) {
  std::map<std::pair<char, char>, bignum> out;
  for(auto p = polymer_template.begin(); p != polymer_template.end(); ++p) {
    auto loc = rules.find(p->first);
    if (loc == rules.end()) {
      out.emplace(*p);
    } else {
      increment(out, std::pair<char, char>{p->first.first, loc->second}, p->second);
      increment(out, std::pair<char, char>{loc->second, p->first.second}, p->second);
      increment(counts, loc->second, p->second);
    }
  }
  return out;
}

int main() {     
  std::string polymer_template_str;
  std::getline(std::cin, polymer_template_str);
  std::map<std::pair<char, char>, bignum>  polymer_template;
  std::map<char, bignum> counts;
  for(auto f = polymer_template_str.begin(), s = polymer_template_str.begin() + 1;
      f != polymer_template_str.end();
      ++f, ++s
      ) {
        increment(polymer_template, std::pair<char, char>{*f, *s});
        increment(counts, *f);
  }

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

  constexpr int count = 40;
  for (int i = 0; i < count; ++i) {
    polymer_template = insert(std::move(polymer_template), counts, rules);
  }
  
  auto [min, max] = std::minmax_element(std::begin(counts), std::end(counts), [&](std::pair<const char, bignum>& a, std::pair<const char, bignum>& b) {
    return a.second < b.second;
  });
  std::cout << max->second - min->second << std::endl;
}