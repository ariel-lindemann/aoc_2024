#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../common/args.hpp"

std::pair<std::vector<int>, std::vector<int>> read_inputs(std::ifstream &file) {
  std::vector<int> left;
  std::vector<int> right;

  int left_num;
  int right_num;

  while (file >> left_num >> right_num) {
    left.push_back(left_num);
    right.push_back(right_num);
  }

  return std::make_pair(left, right);
}

int distance(std::vector<int> sorted_left, std::vector<int> sorted_right) {
  int distance = 0;

  for (size_t i = 0; i < sorted_left.size(); i++) {
    int difference = abs(sorted_left[i] - sorted_right[i]);
    distance += difference;
  }

  return distance;
}

std::unordered_map<int, int> number_frequencies(const std::vector<int> &input) {
  using namespace std;

  unordered_map<int, int> frequencies;

  for (int n : input) {
    if (!frequencies.contains(n)) {
      frequencies[n] = 1;
      continue;
    }
    frequencies[n]++;
  }

  return frequencies;
}

int similarity_score(const std::vector<int> &left,
                     const std::vector<int> &right) {
  int score = 0;
  std::unordered_map<int, int> fequencies = number_frequencies(right);
  for (int n : left) {
    score += n * fequencies[n];
  }

  return score;
}

int main(int argc, char *argv[]) {
  using namespace std;

  std::ifstream file = parse_args(argc, argv);
  // parse file
  auto [left, right] = read_inputs(file);
  file.close();
  // sort vectors
  std::ranges::sort(left);
  std::ranges::sort(right);

  // Part 1
  cout << "Distance: " << distance(left, right) << endl;
  // Part 2
  cout << "Similarity score: " << similarity_score(left, right) << endl;

  return 0;
}
