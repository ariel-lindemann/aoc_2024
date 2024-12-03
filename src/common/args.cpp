#include <iostream>

#include "args.hpp"

std::ifstream parse_args(int argc, char *argv[]) {
  using namespace std;

  if (argc < 2) {
    cerr << "Error: No file path provided" << endl;
    exit(1);
  }

  cout << "Reading file " << argv[1] << " ... " << endl;
  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "Error: Couldn't open file " << argv[1] << endl;
    exit(1);
  }

  return file;
}
