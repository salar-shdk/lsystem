#include "Lsystem.h"
#include "known_lsystems.hpp"
#include <NumCpp.hpp>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  std::vector<rule> rules{rule{"F", "F+F--F+F"}};
  Lsystem dragon_curve("F", rules, 60);
  // std::cout << hilbert.get(stoi(argv[1]) - 5) << "\n";
  std::cout << dragon_curve.get(stoi(argv[1])) << "\n";
  std::cout << "end\n";
  return 0;
}
