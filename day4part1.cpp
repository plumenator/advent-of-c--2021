/*
--- Day 4: Giant Squid ---
You're already almost 1.5km (almost a mile) below the surface of the ocean, already so deep that you can't see any sunlight. What you can see, however, is a giant squid that has attached itself to the outside of your submarine.

Maybe it wants to play bingo?

Bingo is played on a set of boards each consisting of a 5x5 grid of numbers. Numbers are chosen at random, and the chosen number is marked on all boards on which it appears. (Numbers may not appear on all boards.) If all numbers in any row or any column of a board are marked, that board wins. (Diagonals don't count.)

The submarine has a bingo subsystem to help passengers (currently, you and the giant squid) pass the time. It automatically generates a random order in which to draw numbers and a random set of boards (your puzzle input). For example:

7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7
After the first five numbers are drawn (7, 4, 9, 5, and 11), there are no winners, but the boards are marked as follows (shown here adjacent to each other to save space):

22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
After the next six numbers are drawn (17, 23, 2, 0, 14, and 21), there are still no winners:

22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
Finally, 24 is drawn:

22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
At this point, the third board wins because it has at least one complete row or column of marked numbers (in this case, the entire top row is marked: 14 21 17 24 4).

The score of the winning board can now be calculated. Start by finding the sum of all unmarked numbers on that board; in this case, the sum is 188. Then, multiply that sum by the number that was just called when the board won, 24, to get the final score, 188 * 24 = 4512.

To guarantee victory against the giant squid, figure out which board will win first. What will your final score be if you choose that board?
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

  for (auto draw: draws) {
    for (auto& card: cards) {
      card = mark(card, draw);
      if (bingo(card)) {
        std::cout << draw << " * " << sum_unmarked(card) << " = " << sum_unmarked(card) * draw << std::endl;
        goto end;
      }
    }
  }
  end:;
}