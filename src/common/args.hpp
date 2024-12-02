#ifndef COMMON_ARGS_HPP
#define COMMON_ARGS_HPP
#include <fstream>
#include <iostream>

std::ifstream parse_args(int argc, char *argv[]) {
  using namespace std;

  if (argc < 2) {
    cerr << "Error: No file path provided" << endl;
  }

  cout << "Reading file " << argv[1] << " ... " << endl;
  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "Error: Couldn't open file " << argv[1] << endl;
  }

  return file;
}
#endif // COMMON_ARGS_HPP