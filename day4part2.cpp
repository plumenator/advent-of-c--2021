/*
--- Day 4: Giant Squid ---
--- Part Two ---
On the other hand, it might be wise to try a different strategy: let the giant squid win.

You aren't sure how many bingo boards a giant squid could play at once, so rather than waste time counting its arms, the safe thing to do is to figure out which board will win last and choose that one. That way, no matter which boards it picks, it will win for sure.

In the above example, the second board is the last to win, which happens after 13 is eventually called and its middle column is completely marked. If you were to keep playing until this point, the second board would have a sum of unmarked numbers equal to 148 for a final score of 148 * 13 = 1924.

Figure out which board will win last. Once it wins, what would its final score be?
*/

#include <iostream>
#include <string>
#include <array>
#include <cassert>
#include <cmath>
#include <vector>

std::vector<int> split_ints(std::string line, std::string delim=" ") {
  std::vector<int> draws;
  for (;;) {
    line = line[0] == ' ' ? line.substr(1) : line;
    if(auto delim_position = line.find_first_of(delim);
          delim_position != std::string::npos) {
      auto num_str = line.substr(0, delim_position);
      // std::cout << num_str << delim;
      draws.push_back(std::stoi(num_str));
      line = line.substr(delim_position + 1);
    } else {
    break;
    }
  }
  line = line[0] == ' ' ? line.substr(1) : line;
  // std::cout << line << std::endl;
  draws.push_back(std::stoi(line));
  return draws;
}

struct Cell {
  int value = -1;
  bool marked = false;
};

using Card = std::array<std::array<Cell, 5>, 5>;

Card read_card() {
  Card card;
  // std::cout << std::endl;
  for (auto& row: card) {
    std::string line;
    getline(std::cin, line);
    auto nums = split_ints(line);
    int i = 0;
    for (auto& cell: row) {
      cell.value = nums[i];
      ++i;
      // std::cout << cell << ";";
    }
    // std::cout << std::endl;
  }
  return card;
}

Card& mark(Card& card, int draw) {
  for (auto& row: card) {
    for (auto& cell: row) {
      if (cell.value == draw) {
        cell.marked = true;
        return card;
      }
    }
  }
  return card;
}

bool bingo(const Card& card) {
  for (auto& row: card) {
    bool row_marked = true;
    for (auto& cell: row) {
      row_marked = row_marked && cell.marked;
    }
    if (row_marked) {
      return true;
    }
  }
  for (int col = 0; col < 5; ++col) {
    bool col_marked = true;
    for (int row = 0; row < 5; ++row) {
      auto& cell = card[row][col];
      col_marked = col_marked && cell.marked;
    }
    if (col_marked) {
      return true;
    }
  }
  return false;
}

int sum_unmarked(const Card& card) {
  int sum = 0;
  for (auto& row: card) {
    for (auto& cell: row) {
      if (!cell.marked) {
        sum += cell.value;
      }
    }
  }
  return sum;
}

int main() {
  std::string line; 
  std::getline(std::cin, line);
  std::vector<int> draws = split_ints(line, ",");  
  
  std::vector<Card> cards;
  for (std::string line; std::getline(std::cin, line);) {
    cards.push_back(read_card());
  }

  std::vector<int> winning;
  for (auto draw: draws) {
    std::vector<Card> remaining;
    for (auto& card: cards) {
      card = mark(card, draw);
      if (bingo(card)) {
        auto sum = sum_unmarked(card);
        auto score = sum * draw;
        std::cout << draw << " * " << sum << " = " << score << std::endl;
        winning.push_back(score);
      } else {
        remaining.push_back(card);
      }
    }
    cards = remaining;
  }
  std::cout << winning.back() << std::endl;
}