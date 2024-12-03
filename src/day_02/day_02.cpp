#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../common/args.hpp"

std::vector<int> parse_line(const std::string &line) {
  std::istringstream stream(line);
  std::vector<int> numbers;
  while (stream) {
    int n;
    stream >> n;
    numbers.push_back(n);
  }
  return numbers;
}

// returns the index where the problem was found
std::pair<bool, int> line_is_safe(const std::vector<int> &numbers) {
  auto order = numbers[0] <=> numbers[1];

  auto difference_exceeded = [](int a, int b) {
    int difference = abs(a - b);
    return difference < 1 || difference > 3;
  };
  auto changed_order = [order](int a, int b) {
    auto new_order = a <=> b;
    return order != new_order;
  };

  for (size_t i = 0; i < numbers.size() - 2; ++i) {
    if (difference_exceeded(numbers[i], numbers[i + 1])) {
      return std::make_pair(false, i);
    }
    if (changed_order(numbers[i], numbers[i + 1])) {
      return std::make_pair(false, i);
    }
  }
  return std::make_pair(true, -1);
}

std::pair<bool, int> line_is_safe_skip_index(std::vector<int> numbers,
                                             int index) {
  numbers.erase(numbers.begin() + index);
  return line_is_safe(numbers);
}

bool handle_retry(const std::vector<int> &line, int index) {
  auto [result_middle, index_middle] = line_is_safe_skip_index(line, index);
  if (result_middle) {
    return true;
  }
  // Try with the next one
  auto [result_right, index_right] = line_is_safe_skip_index(line, (index + 1));
  if (result_right) {
    return true;
  }
  // Try the previous one
  if (index > 0) {
    auto [result_left, index_left] = line_is_safe_skip_index(line, (index - 1));
    if (result_left) {
      return true;
    }
  }
  return false;
}

int safe_lines(std::ifstream &file, bool problem_dampener) {
  std::string line;
  int safe_lines = 0;
  while (std::getline(file, line)) {
    std::vector<int> line_vector = parse_line(line);

    auto [is_safe, problem_index] = line_is_safe(line_vector);

    if (is_safe) {
      safe_lines++;
    } else if (problem_dampener && handle_retry(line_vector, problem_index)) {
      safe_lines++;
    }
  }
  return safe_lines;
}

int main(int argc, char *argv[]) {
  using namespace std;

  ifstream file;
  // Part 1
  file = parse_args(argc, argv);
  cout << "Safe reports (no dampener): " << safe_lines(file, false) << endl;
  file.close();
  // Part 2
  file = parse_args(argc, argv);
  cout << "Safe reports (with dampener): " << safe_lines(file, true) << endl;
  file.close();
}
