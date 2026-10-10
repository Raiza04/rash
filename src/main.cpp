extern "C" {
#include "linenoise.h"
}

#include "shell.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <vector>

#define RESET "\033[0m"
#define YELLOW "\033[33m"

void lexer(std::string &inputString, std::vector<std::string> &inputVector);

int main() {
  char *line;

  const char *prompt = YELLOW "rash> " RESET;
  while ((line = linenoise(prompt)) != NULL) {

    if (line[0] != '\0') {
      linenoiseHistoryAdd(line);

      std::string inputString(line);

      std::vector<std::string> inputVector;
      lexer(inputString, inputVector);

      if (inputVector.empty()) {
        continue;
      }

      Shell::run(inputVector);
    }

    free(line);
  }

  return 0;
}

void lexer(std::string &inputString, std::vector<std::string> &inputVector) {
  bool in_escape = false;
  bool in_quotes = false;

  std::string buffer = "";

  auto reset_buffer = [&buffer, &inputVector]() {
    if (!buffer.empty()) {
      inputVector.push_back(buffer);
      buffer = "";
    }
  };

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
          reset_buffer();

          if (c == '&') {
            inputVector.push_back("&>");
          } else {
            inputVector.push_back("2>");
          }
          i++;
          continue;
        }

      } else if (c == '<') {
        reset_buffer();

        inputVector.push_back("<");
        continue;

      } else if (c == '>') {
        if (i + 1 < inputString.length() && inputString[i + 1] == '>') {
          reset_buffer();

          inputVector.push_back(">>");
          i++;
          continue;
        } else {
          reset_buffer();

          inputVector.push_back(">");
          continue;
        }
      } else if (c == '|') {
        reset_buffer();

        inputVector.push_back("|");
        continue;
      }
    }

    if (c == ' ' && !in_quotes) {
      reset_buffer();
    } else {
      buffer += c;
    }
  }

  reset_buffer();
}
