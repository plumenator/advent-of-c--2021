/*
--- Day 8: Seven Segment Search ---
You barely reach the safety of the cave when the whale smashes into the cave mouth, collapsing it. Sensors indicate another exit to this cave at a much greater depth, so you have no choice but to press on.

As your submarine slowly makes its way through the cave system, you notice that the four-digit seven-segment displays in your submarine are malfunctioning; they must have been damaged during the escape. You'll be in a lot of trouble without them, so you'd better figure out what's wrong.

Each digit of a seven-segment display is rendered by turning on or offsegments named a through :

  0:      1:      2:      3:      4:
 aaaa    ....    aaaa    aaaa    ....
b    c  .    c  .    c  .    c  b    c
b    c  .    c  .    c  .    c  b    c
 ....    ....    dddd    dddd    dddd
e    f  .    f  e    .  .    f  .    f
e    f  .    f  e    .  .    f  .    f
 gggg    ....    gggg    gggg    ....

  5:      6:      7:      8:      9:
 aaaa    aaaa    aaaa    aaaa    aaaa
b    .  b    .  .    c  b    c  b    c
b    .  b    .  .    c  b    c  b    c
 dddd    dddd    ....    dddd    dddd
.    f  e    f  .    f  e    f  .    f
.    f  e    f  .    f  e    f  .    f
 gggg    gggg    ....    gggg    gggg
So, to rendersegments c and f would be turned on; the rest would be off. T rendersegments a, c, and f would be turned o.

The problem is that the signalssegments have been mixed up on each display The submarine is still trying to display number_lines by producing output on signal wires a through g, but those wiressegments randomly. Worse, th wire/segment connections are mixed up separately for each four-digit display! (All of the digits within a display use the same connections, though.)

So, you might know that only signal wires b and g are turned on, but thatsegments b and g are turned on: the only digitsegments is 1, sosegment c and f are meant to be on. With just that information, you still can't tell which wire (b/g) goes to which segment (c/f). For that, you'll need to collect more information.

For each display, you watch the changing signals for a while, make a note of all ten unique signal patterns you see, and then write down a single four digit output value (your puzzle input). Using the signal patterns, you should be able to work out which pattern corresponds to which digit.

For example, here is what you might see in a single entry in your notes:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf
(The entry is wrapped here to two lines so it fits; in your notes, it will all be on a single line.)

Each entry consists of ten unique signal patterns, a | delimiter, and finally the four digit output value. Within an entry, the same wire/segment connections are used (but you don't know what the connections actually are). The unique signal patterns correspond to the ten different ways the submarine tries to render a digit using the current wire/segment connections. Because 7 is the only digitsegments, dab in the above exampl means that to render a 7, signal lines d, a, and b are on. Because 4 is the only digitsegments, eafb means that to render a 4, signal lines e, a, f and b are on.

Using this information, you should be able to work out which combination of signal wires corresponds to each of the ten digits. Then, you can decode the four digit output value. Unfortunately, in the above example, all of the digits in the output value (cdfeb fcadb cdfebsegments and are mor difficult to deduce.

For now, focus on the easy digits. Consider this larger example:

be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb |
fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec |
fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef |
cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega |
efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga |
gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf |
gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf |
cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd |
ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg |
gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc |
fgae cfgab fg bagce
Because the digits 1, 4, 7, and 8 each use asegments, you should be able t tell which combinations of signals correspond to those digits. Counting only digits in the output values (the part after | on each line), in the above example, there are 26 instances of digits that use asegment (highlighted above).

In the output values, how many times do digits 1, 4, 7, or 8 appear?
*/

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

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

enum class Segments{
  Two = 2, Three, Four, Five, Six, Seven
};

int main() {
  auto identity = [](const std::string& s){
     return std::string(s); 
     };
     
  std::vector<std::string> number_lines;
  for (std::string line; std::getline(std::cin, line);) {
    auto parts = split<std::string>(line, " | ", identity);
    std::cout << line << std::endl;
    std::cout << parts[0] << " | " << parts[1] << std::endl;
    number_lines.push_back(parts[1]);
  }
  const std::set<Segments> unique_count = {
    Segments::Two,
    Segments::Four,
    Segments::Three,
    Segments::Seven
    };

  int occurences = 0;
  for (auto number_line: number_lines) {
  auto digit_signals = split<std::string>(number_line, " ", identity);
    //  std::cout << digit_signals.back() << std::endl;
     for (auto digit_signal: digit_signals)
      if (unique_count.contains(static_cast<Segments>(digit_signal.size())))
        ++occurences;
  }

  std::cout << occurences << std::endl;
}