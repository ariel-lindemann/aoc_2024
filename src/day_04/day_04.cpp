#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>

#include "../common/args.hpp"

std::regex construct_words_regex(const std::string &word, int line_length) {
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

int find_word_occurences(const std::string &file_contents) {
  int occurences = 0;
  const std::string word = "XMAS";
  const int line_length = get_line_length(file_contents);

  auto word_occurences = [file_contents, word](int line_length) {
    std::string word_rev = word;
    std::ranges::reverse(word_rev);

    const std::regex pattern = construct_words_regex(word, line_length);
    const std::regex pattern_rev = construct_words_regex(word_rev, line_length);

    return find_regex_occurences(file_contents, pattern) +
           find_regex_occurences(file_contents, pattern_rev);
  };

  // horizontal
  occurences += word_occurences(0);
  // vertical
  occurences += word_occurences(line_length);
  // diagonal left
  occurences += word_occurences(line_length - 1);
  // diagonal right
  occurences += word_occurences(line_length + 1);

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
