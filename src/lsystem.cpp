#include <algorithm>
#include <cmath>
#include <cstddef>
#include <future>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
// #include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <thread>
#include <vector>

#include "lsystem.h"

using namespace std;

void print_vec3(glm::vec3 vec) {
  cerr << "{ ";
  cerr << vec[0] << ", " << vec[1] << ", " << vec[2];
  cerr << " }, ";
}

void print_vec(vector<glm::vec3> vec) {
  for (auto i : vec) {
    cerr << "{ ";
    print_vec3(i);
    cerr << " }\n";
  }
}

Lsystem::Lsystem(string start, vector<rule> rules, float angle,
                 int num_threads) {
  this->start = start;
  this->rules = rules;
  this->angle = angle;
  this->num_threads = num_threads;
  current_iteration = 0;
  current_string = start;
  current_boundary.min = glm::vec3{0.f};
  current_boundary.max = glm::vec3{0.f};
  current_boundary.center = glm::vec3{0.f};
}

boundary Lsystem::get_boundary() { return this->current_boundary; }

void Lsystem::fix_boundary() {

  this->current_boundary.center.x =
      (this->current_boundary.min.x + this->current_boundary.max.x) / 2.f;
  this->current_boundary.center.y =
      (this->current_boundary.min.y + this->current_boundary.max.y) / 2.f;

  float offset =
      max(this->current_boundary.max.x - this->current_boundary.center.x,
          this->current_boundary.max.y - this->current_boundary.center.y);
  // std::cout << "offset:\t" << offset << "\t" <<
  // to_string(current_boundary.min)
  //<< "\t" << to_string(current_boundary.max) << "\n";
  offset /= 2;
  glm::mat4 scale_matrix =
      glm::scale(glm::mat4{1.f}, glm::vec3{1.f / offset, 1.f / offset, 0.f}) *
      glm::translate(glm::mat4{1.f}, -1.f * this->current_boundary.center);

  for (auto &mat : transformations) {
    // glm::translate(glm::mat4{1.f}, -1.f * glm::vec3(mat[3]));
    mat *= scale_matrix;
    mat[3][0] /= offset;
    mat[3][1] /= offset;
    mat[3][2] /= offset;
  }
}

// cpp thread with string

string Lsystem::get(float iteration) {
  if (static_cast<int>(iteration) >= current_iteration) {
    while (static_cast<int>(iteration) > current_iteration) {
      this->next();
    }
    // TODO: handle floating point iteration
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

string sub_next(size_t start, size_t end, string current_string,
                vector<rule> rules) {
  string output = "";
  // cout << start << "\t" << end << "\n";
  for (size_t i = start; i < end; i++) {
    bool found = false;
    for (auto &rule : rules) {
      try {
        if (rule.sign == current_string.substr(i, rule.sign.size())) {
          output += rule.replacement;
          i += rule.sign.size() - 1;
          found = true;
          break;
        }
      } catch (const out_of_range &e) {
      }
    }
    if (!found)
      output += current_string[i];
  }
  return output;
}

void Lsystem::next() {
  // cout << "next\n";
  current_iteration++;
  string output = "";

  vector<future<string>> results;
  int n_threads =
      min(num_threads, static_cast<int>(1 + current_string.size() / 8));
  // cout << n_threads << "\t" << current_string.size() << "\n";
  for (uint8_t i = 0; i < n_threads; i++) {
    results.emplace_back(async(
        launch::async, sub_next, i * current_string.size() / n_threads,
        (i + 1) * current_string.size() / n_threads, current_string, rules));
  }

  for (auto &result : results)
    output += result.get();

  current_string = output;
}

vector<glm::vec3> sub_geometery(size_t start, size_t end, string current_string,
                                float angle, promise<boundary> boundary_promise,
                                promise<glm::mat4> matrix_promise) {
  vector<glm::vec3> points;
  glm::vec3 last_point{0.f, 0.f, 0.f};
  float delta_angle = angle * PI / 180;
  float current_angle = 0.f;
  boundary current_boundary{glm::vec3{0.f}, glm::vec3{0.f}, glm::vec3{0.f}};

  for (size_t i = start; i < end; i++) {
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
    current_boundary.min.x = min(last_point.x, current_boundary.min.x);
    current_boundary.min.y = min(last_point.y, current_boundary.min.y);
    current_boundary.max.x = max(last_point.x, current_boundary.max.x);
    current_boundary.max.y = max(last_point.y, current_boundary.max.y);
  }

  matrix_promise.set_value(
      glm::translate(glm::mat4{1.f}, last_point) *
      glm::rotate(glm::mat4{1.f}, current_angle, glm::vec3{0.f, 0.f, 1.f}));

  boundary_promise.set_value(current_boundary);
  return points;
}

vector<vector<glm::vec3>> Lsystem::get_geometry(float iteration) {
  this->get(iteration);
  vector<vector<glm::vec3>> output;
  vector<future<vector<glm::vec3>>> results;
  vector<promise<boundary>> boundaries_promise;
  vector<future<boundary>> boundaries_future;
  vector<promise<glm::mat4>> matrix_promise;
  vector<future<glm::mat4>> matrix_future;

  int n_threads =
      min(num_threads, static_cast<int>(1 + current_string.size() / 8));
  // cout << n_threads << "\t" << current_string.size() << "\n";
  for (uint8_t i = 0; i < n_threads; i++) {
    boundaries_promise.emplace_back();
    boundaries_future.push_back(boundaries_promise[i].get_future());
    matrix_promise.emplace_back();
    matrix_future.push_back(matrix_promise[i].get_future());

    results.emplace_back(async(
        launch::async, sub_geometery, i * current_string.size() / n_threads,
        (i + 1) * current_string.size() / n_threads, current_string,
        this->angle, move(boundaries_promise[i]), move(matrix_promise[i])));
  }

  transformations.clear();
  for (size_t i = 0; i < results.size(); i++) {
    vector<glm::vec3> result = results[i].get();
    // output.insert(output.end(), result.begin(), result.end());
    output.push_back(result);

    if (i == 0)
      transformations.push_back(glm::mat4(1.f));
    else {
      transformations.push_back(transformations[i - 1] *
                                matrix_future[i - 1].get());
    }
    // cout << to_string(transformations[i]) << "\n\n";
    boundary sub_boundary = boundaries_future[i].get();
    // cout << "boundary before:\t" << to_string(sub_boundary.min) << "\t"
    //<< to_string(sub_boundary.max) << "\n";
    sub_boundary.min =
        glm::vec3(transformations[i] * glm::vec4(current_boundary.min, 1.f));
    sub_boundary.max =
        glm::vec3(transformations[i] * glm::vec4(current_boundary.max, 1.f));

    // cout << "boundary after:\t" << to_string(sub_boundary.min) << "\t"
    //<< to_string(sub_boundary.max) << "\n";
    current_boundary.min.x = min(current_boundary.min.x,
                                 min(sub_boundary.min.x, sub_boundary.max.x));
    current_boundary.min.y = min(current_boundary.min.y,
                                 min(sub_boundary.min.y, sub_boundary.max.y));
    current_boundary.max.x = max(current_boundary.max.x,
                                 max(sub_boundary.min.x, sub_boundary.max.x));
    current_boundary.max.y = max(current_boundary.max.y,
                                 max(sub_boundary.min.y, sub_boundary.max.y));
  }
  fix_boundary();

  int i = 0;
  // for (auto &t : transformations)
  // cout << i++ << ":\t" << to_string(t) << "\n\n";

  return output;
}
