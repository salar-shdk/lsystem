#include "known_lsystems.hpp"
#include "lsystem.h"
#include <NumCpp.hpp>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  // std::vector<rule> rules{rule{"F", "F+F--F+F"}};
  // Lsystem dragon_curve("F", rules, 60, 15);
  // std::cout << dragon_curve.get(stoi(argv[1])) << "\n";
  std::vector<rule> rules{rule{"F", "FCF"}};
  Lsystem dragon_curve("F", rules, 60, 15);
  Lsystem sierpinski_triangle("F--F--F--", {rule{"F", "F+F--F+F"}}, 60);
  std::cout << sierpinski_triangle.get(stoi(argv[1])) << "\n";
  std::cout << sierpinski_triangle.get_geometry(stoi(argv[1])).size();
  return 0;
}
