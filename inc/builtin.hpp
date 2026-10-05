#pragma once

#include <string>
#include <unordered_set>
#include <vector>

class BuildIn {
public:
  BuildIn(std::vector<std::string> &inputStr) : input(inputStr) {};
  bool execute();

private:
  std::vector<std::string> input;
  std::unordered_set<std::string> builtInCommands = {"cd", "export", "alias",
                                                     "history"};
};
