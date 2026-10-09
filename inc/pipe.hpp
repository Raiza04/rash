#pragma once

#include <string>
#include <vector>

class Pipe {
public:
  Pipe() = default;
  ~Pipe() = default;

  static bool check_and_execute_pipe(std::vector<std::string> &input);
  static std::vector<std::vector<std::string>>
  divide_input_vector(const std::vector<std::string> &input);
};
