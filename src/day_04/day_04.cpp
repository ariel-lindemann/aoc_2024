#include <fstream>
#include <iostream>
#include <regex>

#include "../common/args.hpp"

std::regex construct_regex(const std::string &word, int line_length) {
  const std::string length_str = std::to_string(line_length);

  std::string word_reversed = word;
  std::reverse(word_reversed.begin(), word_reversed.end());

  auto construct_regex_string = [&length_str](const std::string &s) {
    // positive lookahead
    std::string constructed = "(?=";
    for (auto it = s.begin(); it != s.end(); ++it) {
      char c = *it;
      constructed += std::string(1, c);
      if (std::next(it) != s.end()) {
        constructed += "((.|\\n){" + length_str + "})";
      }
    }
    constructed += ")";
    return constructed;
  };

  const std::string regex_original = construct_regex_string(word);
  const std::string regex_reversed = construct_regex_string(word_reversed);

  return std::regex(regex_original);
}

int find_regex_occurences(const std::string &str, std::regex pattern) {
  auto matches_begin = std::sregex_iterator(str.begin(), str.end(), pattern);
  auto matches_end = std::sregex_iterator();

  int count = std::distance(matches_begin, matches_end);

  return count;
}

int vertical_occurences(const std::string &str) {
  int line_length = str.find('\n');
  const std::regex pattern = construct_regex("XMAS", line_length);
  const std::regex pattern_reverse = construct_regex("SAMX", line_length);

  return find_regex_occurences(str, pattern) +
         find_regex_occurences(str, pattern_reverse);
}

int horizontal_occurences(std::string str) {
  const std::regex pattern("XMAS");
  const std::regex pattern_reverse("SAMX");

  return find_regex_occurences(str, pattern) +
         find_regex_occurences(str, pattern_reverse);
}

int diagonal_left(const std::string &str) {
  int line_length = str.find('\n');
  const std::regex pattern = construct_regex("XMAS", line_length - 1);
  const std::regex pattern_reverse = construct_regex("SAMX", line_length - 1);

  int x = find_regex_occurences(str, pattern);
  int y = find_regex_occurences(str, pattern_reverse);
  std::cout << "diagonal left " << x + y << std::endl;
  return x + y;
}

int diagonal_right(const std::string &str) {
  int line_length = str.find('\n');
  const std::regex pattern = construct_regex("XMAS", line_length + 1);
  const std::regex pattern_reverse = construct_regex("SAMX", line_length + 1);

  int x = find_regex_occurences(str, pattern);
  int y = find_regex_occurences(str, pattern_reverse);
  std::cout << "diagonal right " << x + y << std::endl;
  return x + y;
}

int find_word_occurences(std::ifstream &file) {
  int occurences = 0;
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  std::string str = buffer.str();

  occurences += horizontal_occurences(str);
  occurences += vertical_occurences(str);
  occurences += diagonal_left(str);
  occurences += diagonal_right(str);

  return occurences;
}

int main(int argc, char *argv[]) {
  std::ifstream file = parse_args(argc, argv);
  // Part 1
  std::cout << "XMAS appears " << find_word_occurences(file) << " times."
            << std::endl;
}
