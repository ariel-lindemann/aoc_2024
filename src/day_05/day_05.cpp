#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "../common/args.hpp"

using Rules = std::unordered_map<int, std::vector<int>>;

bool validate_line(const std::vector<int> &line, Rules &rules) {
  std::set<int> forbidden_numbers;

  for (int n : line) {
    if (forbidden_numbers.contains(n)) {
      return false;
    }
    // check if empty
    if (rules.find(n) == rules.end()) {
      continue;
    }
    forbidden_numbers.insert(rules[n].begin(), rules[n].end());
  }
  return true;
}

int middle_number(const std::vector<int> &line) {
  return line[line.size() / 2];
}

int handle_line(std::string &line, Rules &rules) {
  std::istringstream stream(line);
  std::vector<int> numbers;
  std::string number_str;

  while (std::getline(stream, number_str, ',')) {
    numbers.push_back(std::stoi(number_str));
  }
  if (validate_line(numbers, rules)) {
    return middle_number(numbers);
  }
  return 0;
}

Rules parse_rules(std::ifstream &file) {
  Rules rules;
  std::string line;
  std::regex pattern(R"((\d+)\|(\d+))");
  std::smatch match;
  // Parse rules
  while (std::getline(file, line)) {
    if (!std::regex_match(line, match, pattern)) {
      break; // Exit the loop if the line doesn't match
    }
    int a = std::stoi(match[1]);
    int b = std::stoi(match[2]);

    rules[b].push_back(a);
  }
  return rules;
}

int parse_file(std::ifstream &file) {
  Rules rules = parse_rules(file);

  std::string line;
  int sum = 0;

  while (std::getline(file, line)) {
    sum += handle_line(line, rules);
  }
  return sum;
}

int main(int argc, char *argv[]) {
  std::ifstream file = parse_args(argc, argv);
  // Part 1
  std::cout << "Result " << parse_file(file) << std::endl;
}
