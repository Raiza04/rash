#include "builtin.hpp"
#include <filesystem>

namespace fs = std::filesystem;

bool BuildIn::execute() {
  if (builtInCommands.contains(input[0])) {
    fs::current_path(input[1]);
    return true;
  }
  return false;
};
