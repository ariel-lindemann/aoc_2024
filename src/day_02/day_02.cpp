#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../common/args.hpp"

enum class line_status {
  safe,
  difference_ecxeeded,
  changed_order,
};

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

// returns the index where the problem was found. -1 means no problem
std::pair<line_status, int> line_is_safe(const std::vector<int> &numbers) {
  auto order = numbers[0] <=> numbers[1];

  auto difference_exceeded = [](int a, int b) {
    int difference = abs(a - b);
    return difference < 1 || difference > 3;
  };
  auto changed_order = [order](int a, int b) {
    auto new_order = a <=> b;
    return order != new_order;
  };

  for (int i = 0; i < numbers.size() - 2; ++i) {
    if (difference_exceeded(numbers[i], numbers[i + 1])) {
      std::cout << "Unsafe! Change to steep " << numbers[i] << " - "
                << numbers[i + 1] << " Current index: " << i << std::endl;
      return std::make_pair(line_status::difference_ecxeeded, i);
    }
    if (changed_order(numbers[i], numbers[i + 1])) {
      std::cout << "Unsafe! Changed direction"
                << " Current index: " << i << std::endl;
      return std::make_pair(line_status::changed_order, i);
    }
  }
  return std::make_pair(line_status::safe, -1);
}

std::pair<line_status, int> line_is_safe_skip_index(std::vector<int> numbers,
                                                    int index) {
  std::cout << "Removed " << numbers.at(index) << " from index " << index
            << std::endl;
  numbers.erase(numbers.begin() + index);
  return line_is_safe(numbers);
}

int safe_lines(std::ifstream &file, bool problem_dampener) {
  std::string line;
  int safe_lines = 0;
  while (std::getline(file, line)) {
    std::cout << "Checking line " << line << std::endl;
    std::vector<int> line_vector = parse_line(line);

    auto [result, problem_index] = line_is_safe(line_vector);

    if (result == line_status::safe) {
      std::cout << "Safe!" << std::endl;
      safe_lines++;
    } else if (problem_dampener) {
      auto [result_left, index_left] = line_is_safe_skip_index(line_vector, problem_index);
      if (result == line_status::safe) {
        std::cout << "Safe!" << std::endl;
        safe_lines++;
        continue;
      }
      // Try with the next one
      auto [result_right, index_right] =
          line_is_safe_skip_index(line_vector, (problem_index + 1));
      if (result == line_status::safe) {
        std::cout << "Safe!" << std::endl;
        safe_lines++;
        continue;
      }
    }
  }
  return safe_lines;
}

int main(int argc, char *argv[]) {
  using namespace std;

  ifstream file;
  // Part 1
  /*
  file = parse_args(argc, argv);
  cout << "Safe reports (no dampener): " << safe_lines(file, false) << endl;
  file.close();
  */
  // Part 2
  file = parse_args(argc, argv);
  cout << "Safe reports (with dampener): " << safe_lines(file, true) << endl;
  file.close();
}
