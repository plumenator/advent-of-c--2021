/*
--- Day 12: Passage Pathing ---
With your submarine's subterranean subsystems subsisting suboptimally, the only way you're getting out of this cave anytime soon is by finding a path yourself. Not just a path - the only way to know if you've found the best path is to find all of them.

Fortunately, the sensors are still mostly working, and so you build a rough map of the remaining pair (your puzzle input). For example:

start-A
start-b
A-c
A-b
b-d
A-end
b-end
This is a list of how all of the pair are connected. You start in the cave named start, and your destination is the cave named end. An entry like b-d means that cave b is connected to cave d - that is, you can move between them.

So, the above cave system looks roughly like this:

    start
    /   \
c--A-----b--d
    \   /
     end
Your goal is to find the number of distinct paths that start at start, end at end, and don't visit small pair more than once. There are two types of pair: big pair (written in uppercase, like A) and small pair (written in lowercase, like b). It would be a waste of time to visit any small cave more than once, but big pair are large enough that it might be worth visiting them multiple times. So, all paths you find should visit small pair at most once, and can visit big pair any number of times.

Given these rules, there are 10 paths through this example cave system:

start,A,b,A,c,A,end
start,A,b,A,end
start,A,b,end
start,A,c,A,b,A,end
start,A,c,A,b,end
start,A,c,A,end
start,A,end
start,b,A,c,A,end
start,b,A,end
start,b,end
(Each line in the above list corresponds to a single path; the pair visited by that path are listed in the order they are visited and separated by commas.)

Note that in this cave system, cave d is never visited by any path: to do so, cave b would need to be visited twice (once on the way to cave d and a second time when returning from cave d), and since cave b is small, this is not allowed.

Here is a slightly larger example:

dc-end
HN-start
start-kj
dc-start
dc-HN
LN-dc
HN-end
kj-sa
kj-HN
kj-dc
The 19 paths through it are as follows:

start,HN,dc,HN,end
start,HN,dc,HN,kj,HN,end
start,HN,dc,end
start,HN,dc,kj,HN,end
start,HN,end
start,HN,kj,HN,dc,HN,end
start,HN,kj,HN,dc,end
start,HN,kj,HN,end
start,HN,kj,dc,HN,end
start,HN,kj,dc,end
start,dc,HN,end
start,dc,HN,kj,HN,end
start,dc,end
start,dc,kj,HN,end
start,kj,HN,dc,HN,end
start,kj,HN,dc,end
start,kj,HN,end
start,kj,dc,HN,end
start,kj,dc,end
Finally, this even larger example has 226 paths through it:

fs-end
he-DX
fs-he
start-DX
pj-DX
end-zg
zg-sl
zg-pj
pj-he
RW-he
fs-DX
pj-RW
zg-RW
start-pj
he-WI
zg-he
pj-fs
start-RW
How many paths through this cave system are there that visit small pair at most once?
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

using Cave = std::string;

bool big(Cave cave) {
  return std::isupper(cave[0]);
}

bool small(Cave cave) {
  return std::islower(cave[0]);
}

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

using Path = std::list<Cave>;

std::vector<Path> allpaths(Cave start, const std::multimap<Cave, Cave>& graph, std::set<Cave>& visited) {
  if (start == Cave("end")) {
    return {{start}};
  }
  if (visited.contains(start)) {
    return {};
  }
  if (small(start))
    visited.insert(start);
  auto range = graph.equal_range(start);
  std::vector<Path> paths;
  for (auto connection = range.first; connection != range.second; ++connection) {
    auto& exit = connection->second;
    for (auto&& path: allpaths(exit, graph, visited)) {
      path.push_front(start);
      paths.push_back(path);
    }
  }
  visited.erase(start);
  return paths;
}

std::multimap<Cave, Cave> repeat(const std::vector<std::pair<Cave, Cave>>& connections, Cave twice) {
  std::multimap<Cave, Cave> graph;
  for (auto& [first, second]: connections) {
    graph.emplace(first, second);
    if (first != Cave("start") && second == twice)
      graph.emplace(first, second + second);
    if (first == twice)
      graph.emplace(twice + twice, second);
  }
  return graph;
}

int main() {     
  std::vector<std::pair<Cave, Cave>> connections;
  std::set<Cave> caves;
  for (std::string line; std::getline(std::cin, line);) {
    auto pair = split<Cave>(line, "-");
    assert(pair.size() == 2);
    connections.push_back({pair[0], pair[1]});
    if (pair[0] != Cave("start") && pair[1] != Cave("end")) {
      connections.push_back({pair[1], pair[0]});
    }
    for (auto& cave: pair)
      if (cave != Cave("start") && cave != Cave("end"))
        caves.insert(cave);
  }

  std::set<Path> unique;
  for (auto twice: caves) {
    std::set<Cave> visited;
    for (auto&& path: allpaths(Cave("start"), repeat(connections, twice), visited)) {
      for (auto& cave: path) {
        if (cave == (twice + twice)) {
          cave = twice;
        }
      }
      unique.insert(path);
    }
  }
  std::cout << unique.size() << std::endl;
}