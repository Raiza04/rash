#pragma once

#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

class Shell {
public:
  Shell(std::vector<std::string> &inputStr) : input(inputStr) {};
  void run();

private:
  std::vector<std::string> input;
};
