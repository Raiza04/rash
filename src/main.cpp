#include "shell.hpp"
#include <iostream>
#include <string>
#include <vector>

#define RESET "\033[0m"
#define YELLOW "\033[33m"

void parser(std::string &inputString, std::vector<std::string> &inputVector);

int main() {
  while (true) {
    std::cout << YELLOW << "rash> " << RESET << std::flush;

    std::string inputString;
    std::getline(std::cin, inputString);

    if (std::cin.eof()) {
      break;
    }

    if (inputString.empty()) {
      continue;
    }

    std::vector<std::string> currentInput;

    parser(inputString, currentInput);

    if (currentInput.empty()) {
      continue;
    }

    Shell shell(currentInput);
    shell.run();
  }

  return 0;
}

void parser(std::string &inputString, std::vector<std::string> &inputVector) {
  bool in_escape = false;
  bool in_quotes = false;

  std::string buffer = "";

  for (size_t i = 0; i < inputString.length(); i++) {
    char c = inputString[i];

    if (c == '"') {
      in_quotes = !in_quotes;
      continue;
    } else if (c == '\\') {
      if (!in_quotes) {
        in_escape = true;
        continue;
      }
    }

    if (in_escape) {
      buffer += c;
      in_escape = false;
      continue;
    }

    if (!in_quotes) {
      if (c == '2' || c == '&') {
        if (i + 1 < inputString.length() && inputString[i + 1] == '>') {
          if (!buffer.empty()) {
            inputVector.push_back(buffer);
            buffer = "";
          }
          if (c == '&') {
            inputVector.push_back("&>");
          } else {
            inputVector.push_back("2>");
          }
          i++;
          continue;
        }

      } else if (c == '<') {
        if (!buffer.empty()) {
          inputVector.push_back(buffer);
          buffer = "";
        }

        inputVector.push_back("<");
        continue;

      } else if (c == '>') {
        if (i + 1 < inputString.length() && inputString[i + 1] == '>') {
          if (!buffer.empty()) {
            inputVector.push_back(buffer);
            buffer = "";
          }
          inputVector.push_back(">>");
          i++;
          continue;
        } else {
          if (!buffer.empty()) {
            inputVector.push_back(buffer);
            buffer = "";
          }
          inputVector.push_back(">");
          continue;
        }
      }
    }

    if (c == ' ' && !in_quotes) {
      if (!buffer.empty()) {
        inputVector.push_back(buffer);
        buffer = "";
      }
    } else {
      buffer += c;
    }
  }

  if (!buffer.empty()) {
    inputVector.push_back(buffer);
  }
}
