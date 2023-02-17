#include <algorithm>
#include <cppitertools/itertools.hpp>
#include <cstddef>
#include <string>
#include <vector>

#include "Lsystem.h"

using namespace std;

Lsystem::Lsystem(std::string start, std::vector<rule> rules, float angle) {
  this->start = start;
  this->rules = rules;
  this->angle = angle;
  current_iteration = 0;
  current_string = start;
}

std::string Lsystem::get(float iteration) {
  if (static_cast<int>(iteration) >= current_iteration) {
    while (static_cast<int>(iteration) > current_iteration) {
      this->next();
    }
    // handle floating point iteration
    return current_string;
  } else {
    current_iteration = 0;
    current_string = start;
    return this->get(iteration);
  }
}

void Lsystem::next() {
  std::cout << "next\n";
  current_iteration++;
  std::string output = "";
  for (size_t i = 0; i < current_string.size(); i++) {
    bool found = false;
    for (auto &rule : rules) {
      try {
        if (rule.sign == current_string.substr(i, rule.sign.size())) {
          output += rule.replacement;
          i += rule.sign.size() - 1;
          found = true;
          break;
        }
      } catch (const std::out_of_range &e) {
      }
    }
    if (!found)
      output += current_string[i];
  }
  current_string = output;
}
