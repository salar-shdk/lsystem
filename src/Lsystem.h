#ifndef LSYSTEM_H_
#define LSYSTEM_H_

#include <NumCpp.hpp>
#include <string>
#include <vector>

struct rule {
  std::string sign;
  std::string replacement;
};

class Lsystem {
public:
  Lsystem(std::string start, std::vector<rule> rules, float angle);

  std::string get(float iteration);

private:
  std::string start;
  std::vector<rule> rules;
  float angle;
  int current_iteration;
  std::string current_string;

  void next();
};

#endif
