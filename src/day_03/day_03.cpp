#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#include "../common/args.hpp"

constexpr std::string_view DO = "do()";
constexpr std::string_view DONT = "don't()";

int read_commands(std::string text, bool do_functionality) {
  int product = 0;
  bool do_status = true;

  std::regex pattern(R"(mul\((\d+),(\d+)\)|(do\(\))|don't\(\))");

  auto text_begin = std::sregex_iterator(text.begin(), text.end(), pattern);
  auto text_end = std::sregex_iterator();

  for (auto it = text_begin; it != text_end; ++it) {
    std::smatch match = *it;

    if (std::string match_string = match.str(); match_string == DO) {
      do_status = true;
      continue;
    } else if (match_string == DONT) {
      do_status = false;
      continue;
    }

    int a = std::stoi(match[1].str());
    int b = std::stoi(match[2].str());

    if (do_status || !do_functionality) {
      product += a * b;
    }
  }

  return product;
}

int main(int argc, char *argv[]) {

  std::ifstream file = parse_args(argc, argv);
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  std::string text = buffer.str();

  // Part 1
  std::cout << "Product (without do/don't instructions): \n"
            << read_commands(text, false) << std::endl;
  // Part 2
  file = parse_args(argc, argv);
  std::cout << "Product (with do/don't instructions): \n"
            << read_commands(text, true) << std::endl;
}
