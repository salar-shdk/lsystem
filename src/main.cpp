#include "Lsystem.h"
#include "known_lsystems.hpp"
#include <NumCpp.hpp>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  std::vector<rule> rules{rule{"FX", "X+YF+"}, rule{"Y", "-FX-Y"}};
  Lsystem dragon_curve("FX", rules, 90);
  std::cout << hilbert.get(stoi(argv[1]) - 5) << "\n";
  std::cout << dragon_curve.get(stoi(argv[1])) << "\n";
}
