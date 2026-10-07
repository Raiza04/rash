#include "shell.hpp"
#include "builtin.hpp"
#include "redirect.hpp"
#include <cstdio>
#include <cstdlib>

void Shell::run() {
  if (BuiltIn::check_and_execute(input)) {
    return;
  }

  pid_t processId = fork();

  if (processId == -1) {
    perror("Failed to start the process: \n");

  } else if (processId == 0) {
    if (!Redirect::check_and_redirect(input)) {
      exit(EXIT_FAILURE);
    };

    std::vector<char *> args;
    args.reserve(input.size() + 1);

    for (size_t i = 0; i < input.size(); i++) {
      args.push_back(input[i].data());
    }
    args.push_back(nullptr);

    if (execvp(args[0], args.data()) == -1) {
      perror("Failed to start the process: \n");
      std::exit(EXIT_FAILURE);
    }

  } else if (processId > 0) {
    waitpid(processId, NULL, 0);
  }
};
