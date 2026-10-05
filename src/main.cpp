#include "shell.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define RESET "\033[0m"
#define YELLOW "\033[33m"

int main() {
  while (true) {
    std::cout << YELLOW << "rash> " << RESET << std::flush;

    std::string inputString;
    std::getline(std::cin, inputString);

    if (std::cin.eof() || inputString == "exit") {
      break;
    }

    if (inputString.empty()) {
      continue;
    }

    std::istringstream stream(inputString);
    std::string word;

    std::vector<std::string> currentInput;

    while (stream >> word) {
      currentInput.push_back(word);
    }

    if (currentInput.empty()) {
      continue;
    }

    Shell shell(currentInput);
    shell.run();
  }

  return 0;
}
