#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

#include "../common/args.hpp"

std::regex construct_regex(const std::string &word, int line_length) {
  const std::string length_str = std::to_string(line_length);

  auto construct_regex_string = [&length_str](std::string_view s) {
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

  const std::string regex_pattern = construct_regex_string(word);

  return std::regex(regex_pattern);
}

int find_regex_occurences(const std::string &str, const std::regex &pattern) {
  auto matches_begin = std::sregex_iterator(str.begin(), str.end(), pattern);
  auto matches_end = std::sregex_iterator();

  auto count = static_cast<int>(std::distance(matches_begin, matches_end));

  return count;
}

int get_line_length(std::string_view s) {
  return static_cast<int>(s.find('\n'));
}

std::pair<std::regex, std::regex> regex_pairs(const std::string &word,
                                              int line_length) {
  std::string word_rev = word;
  std::ranges::reverse(word_rev);

  const std::regex pattern = construct_regex(word, line_length);
  const std::regex pattern_rev = construct_regex(word_rev, line_length);
  return std::make_pair(pattern, pattern_rev);
}

int vertical_occurences(const std::string &str) {
  const int line_length = get_line_length(str);
  auto [pattern, pattern_reverse] = regex_pairs("XMAS", line_length);

  return find_regex_occurences(str, pattern) +
         find_regex_occurences(str, pattern_reverse);
}

int horizontal_occurences(const std::string &str) {
  const std::regex pattern("XMAS");
  const std::regex pattern_reverse("SAMX");

  return find_regex_occurences(str, pattern) +
         find_regex_occurences(str, pattern_reverse);
}

int diagonal_left(const std::string &str) {
  const int line_length = get_line_length(str);
  auto [pattern, pattern_reverse] = regex_pairs("XMAS", line_length - 1);

  int x = find_regex_occurences(str, pattern);
  int y = find_regex_occurences(str, pattern_reverse);

  return x + y;
}

int diagonal_right(const std::string &str) {
  const int line_length = get_line_length(str);
  auto [pattern, pattern_reverse] = regex_pairs("XMAS", line_length + 1);

  int x = find_regex_occurences(str, pattern);
  int y = find_regex_occurences(str, pattern_reverse);

  return x + y;
}

int find_word_occurences(const std::string &file_contents) {
  int occurences = 0;

  occurences += horizontal_occurences(file_contents);
  occurences += vertical_occurences(file_contents);
  occurences += diagonal_left(file_contents);
  occurences += diagonal_right(file_contents);

  return occurences;
}

std::regex construct_x_mas_regex(const char tl, const char tr, const char bl,
                                 const char br, int line_length) {
  const std::string TOP_LEFT(1, tl);
  const std::string TOP_RIGHT(1, tr);
  const std::string CENTER = "A";
  const std::string BOTTOM_LEFT(1, bl);
  const std::string BOTTOM_RIGHT(1, br);

  const std::string D = std::to_string(line_length - 1);

  const std::string pattern = "(?=" + TOP_LEFT + "(.)" + TOP_RIGHT +
                              "(.|\\n){" + D + "}" + CENTER + "(.|\\n){" + D +
                              "}" + BOTTOM_LEFT + "(.)" + BOTTOM_RIGHT + ")";
  return std::regex(pattern);
}

int find_x_mas_occurences(const std::string &file_contents) {
  char combinations[4][4] = {{'M', 'M', 'S', 'S'},
                             {'M', 'S', 'M', 'S'},
                             {'S', 'M', 'S', 'M'},
                             {'S', 'S', 'M', 'M'}};
  int occurences = 0;
  const int line_length = get_line_length(file_contents);

  for (auto c : combinations) {
    occurences += find_regex_occurences(
        file_contents,
        construct_x_mas_regex(c[0], c[1], c[2], c[3], line_length));
  }
  return occurences;
}

int main(int argc, char *argv[]) {
  std::ifstream file = parse_args(argc, argv);
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  std::string str = buffer.str();
  // Part 1
  std::cout << "XMAS word appears " << find_word_occurences(str) << " times."
            << std::endl;
  // Part 2
  std::cout << "X-MAS pattern appears " << find_x_mas_occurences(str)
            << " times." << std::endl;
}
