#include "known_lsystems.hpp"
#include "lsystem.h"
#include <NumCpp.hpp>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  // std::vector<rule> rules{rule{"F", "F+F--F+F"}};
  // Lsystem dragon_curve("F", rules, 60, 15);
  // std::cout << dragon_curve.get(stoi(argv[1])) << "\n";
  std::vector<rule> rules{rule{"F", "FCF"}};
  Lsystem dragon_curve("F", rules, 60, 15);
  Lsystem sierpinski_triangle("F", {rule{"F", "F+F--F+F"}}, 60);
  std::chrono::time_point<std::chrono::system_clock> time_a =
      std::chrono::system_clock::now();
  // for (auto ptr : sierpinski_triangle.get(stoi(argv[1])))
  // cout << *ptr;
  sierpinski_triangle.get(stoi(argv[1]));
  std::chrono::time_point<std::chrono::system_clock> time_b =
      std::chrono::system_clock::now();
  std::cout << "get:" << (time_b - time_a).count() << "\n";
  std::cout << sierpinski_triangle.get_geometry(stoi(argv[1])).size();
  std::chrono::time_point<std::chrono::system_clock> time_c =
      std::chrono::system_clock::now();
  std::cout << "get geom:" << (time_c - time_b).count() << "\n";

  return 0;
}
