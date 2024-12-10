#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../common/args.hpp"

using Rules = std::unordered_map<int, std::unordered_set<int>>;

bool validate_line(const std::vector<int> &line, Rules &rules) {
  std::unordered_set<int> forbidden_numbers;

  for (int n : line) {
    if (forbidden_numbers.contains(n)) {
      return false;
    }
    if (!rules.contains(n)) {
      continue;
    }
    forbidden_numbers.insert(rules[n].begin(), rules[n].end());
  }
  return true;
}

int middle_number(const std::vector<int> &line) {
  return line[line.size() / 2];
}

void apply_rule(std::vector<int> &line, int n,
                const std::unordered_set<int> &successors) {
  const auto n_index = std::distance(line.begin(), std::ranges::find(line, n));

  std::vector<int> out_of_place;

  for (auto it = line.begin() + n_index; it != line.end();) {
    int e = *it;
    if (successors.contains(e)) {
      out_of_place.push_back(e);
      it = line.erase(it); // Erase returns the next valid iterator
    } else {
      ++it; // Increment the iterator only when not erasing
    }
  }
  line.insert(line.begin() + n_index, out_of_place.begin(), out_of_place.end());
}

void fix_line_ordering(std::vector<int> &line, Rules &rules) {
  while (!validate_line(line, rules)) {
    for (int number : line) {
      if (rules.contains(number)) {
        apply_rule(line, number, rules[number]);
      }
    }
  }
}

int handle_line(const std::string &line, Rules &rules, bool correct_ordering) {
  std::istringstream stream(line);
  std::vector<int> numbers;
  std::string number_str;

  while (std::getline(stream, number_str, ',')) {
    numbers.push_back(std::stoi(number_str));
  }
  if (!correct_ordering && validate_line(numbers, rules)) {
    return middle_number(numbers);
  } else if (correct_ordering && !validate_line(numbers, rules)) {
    fix_line_ordering(numbers, rules);
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

    rules[b].insert(a);
  }
  return rules;
}

int parse_file(std::ifstream &file, bool correct_ordering) {
  Rules rules = parse_rules(file);

  std::string line;
  int sum = 0;

  while (std::getline(file, line)) {
    sum += handle_line(line, rules, correct_ordering);
  }
  return sum;
}

int main(int argc, char *argv[]) {
  std::ifstream file = parse_args(argc, argv);
  // Part 1
  std::cout << "Part 1: " << parse_file(file, false) << std::endl;
  file.close();
  // Part 2
  file = parse_args(argc, argv);
  std::cout << "Part 2: " << parse_file(file, true) << std::endl;
  file.close();
}
