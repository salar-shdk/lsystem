#ifndef LSYSTEM_H_
#define LSYSTEM_H_

#include <NumCpp.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

const double PI = std::atan(1.0) * 4;

struct boundary {
  glm::vec3 min;
  glm::vec3 max;
  glm::vec3 center;
};

struct rule {
  std::string sign;
  std::string replacement;
};

class Lsystem {
public:
  Lsystem(std::string start, std::vector<rule> rules, float angle);

  std::string get(float iteration);
  std::vector<glm::vec3> get_geometry(float iteration);
  boundary get_boundary();

private:
  std::string start;
  std::vector<rule> rules;
  float angle;
  int current_iteration;
  std::string current_string;
  boundary current_boundary;

  void next();
};

#endif
