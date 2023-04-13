#include <algorithm>
#include <cmath>
#include <cstddef>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>
#include <string>
#include <vector>

#include "lsystem.h"

using namespace std;

void print_vec3(glm::vec3 vec) {
  std::cerr << "{ ";
  std::cerr << vec[0] << ", " << vec[1] << ", " << vec[2];
  std::cerr << " }, ";
}

void print_vec(std::vector<glm::vec3> vec) {
  for (auto i : vec) {
    std::cerr << "{ ";
    print_vec3(i);
    std::cerr << " }\n";
  }
}

Lsystem::Lsystem(std::string start, std::vector<rule> rules, float angle) {
  this->start = start;
  this->rules = rules;
  this->angle = angle;
  current_iteration = 0;
  current_string = start;
  current_boundary.min = glm::vec3{0.f};
  current_boundary.max = glm::vec3{0.f};
  current_boundary.center = glm::vec3{0.f};
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
    current_boundary.min = glm::vec3{0.f};
    current_boundary.max = glm::vec3{0.f};
    current_boundary.center = glm::vec3{0.f};
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

boundary Lsystem::get_boundary() {
  this->current_boundary.center.x =
      (this->current_boundary.min.x + this->current_boundary.max.x) / 2.f;
  this->current_boundary.center.y =
      (this->current_boundary.min.y + this->current_boundary.max.y) / 2.f;

  return this->current_boundary;
}

std::vector<glm::vec3> Lsystem::get_geometry(float iteration) {
  this->get(iteration);

  std::vector<glm::vec3> points;
  glm::vec3 last_point{0.f, 0.f, 0.f};
  float delta_angle = this->angle * PI / 180;
  float current_angle = 0.f;

  for (size_t i = 0; i < current_string.size(); i++) {
    if (current_string[i] == '-') {
      current_angle -= delta_angle;
    } else if (current_string[i] == '+') {
      current_angle += delta_angle;
    } else if (current_string[i] == 'f') {
      last_point[0] += cosf(current_angle);
      last_point[1] += sinf(current_angle);
    } else if (current_string[i] == 'F') {
      points.push_back(last_point);
      last_point[0] += cosf(current_angle);
      last_point[1] += sinf(current_angle);
      points.push_back(last_point);
    }
    if (last_point.x < this->current_boundary.min.x)
      this->current_boundary.min.x = last_point.x;
    if (last_point.y < this->current_boundary.min.y)
      this->current_boundary.min.y = last_point.y;
    if (last_point.x > this->current_boundary.max.x)
      this->current_boundary.max.x = last_point.x;
    if (last_point.y > this->current_boundary.max.y)
      this->current_boundary.max.y = last_point.y;
  }
  return points;
}
