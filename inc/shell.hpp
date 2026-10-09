#pragma once

#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

class Shell {
public:
  Shell() = default;
  ~Shell() = default;

  static void run(std::vector<std::string> &input);
};
