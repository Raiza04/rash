#include "pipe.hpp"
#include "redirect.hpp"
#include <cstdlib>
#include <iostream>
#include <sched.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using std::vector, std::string;

bool Pipe::check_and_execute_pipe(vector<string> &input) {
  vector<vector<string>> pipe_inputs = divide_input_vector(input);

  if (pipe_inputs.size() == 1) {
    return false;
  }

  for (vector<string> input_str : pipe_inputs) {
    if (input_str.empty()) {
      std::cerr << "Error: Use of invalid pipe syntax. Check the command\n";
      return true;
    }
  }

  int fd_in = 0;

  for (size_t i = 0; i < pipe_inputs.size(); i++) {
    int fd[2];

    if (i + 1 < pipe_inputs.size()) {
      if (pipe(fd) == -1) {
        std::cerr << "Unexpected pipe error\n";
        return true;
      }
    }

    pid_t pid = fork();

    if (pid == -1) {
      std::cerr << "Fork failed\n";

    } else if (pid == 0) {
      if (dup2(fd_in, STDIN_FILENO) < 0) {
        std::cerr << "Failed to redirect the standard input\n";
        close(fd_in);
        if (i + 1 < pipe_inputs.size()) {
          close(fd[1]);
        }
        exit(EXIT_FAILURE);
      }

      if (i + 1 < pipe_inputs.size()) {
        if (dup2(fd[1], STDOUT_FILENO) < 0) {
          std::cerr << "Failed to redirect the standard output\n";
          close(fd_in);
          close(fd[1]);
          exit(EXIT_FAILURE);
        }
      }

      Redirect::check_and_redirect(pipe_inputs[i]);

      vector<char *> args;
      args.reserve(pipe_inputs[i].size() + 1);

      for (string &word : pipe_inputs[i]) {
        args.push_back(word.data());
      }
      args.push_back(nullptr);

      if (execvp(args[0], args.data())) {
        std::cerr << "Command not found\n";
        exit(EXIT_FAILURE);
      }
    } else {
      if (fd_in != 0)
        close(fd_in);
      if (i + 1 < pipe_inputs.size()) {
        close(fd[1]);
        fd_in = fd[0];
      }
    }
  }

  while (wait(NULL) > 0)
    ;

  return true;
}

vector<vector<string>> Pipe::divide_input_vector(const vector<string> &input) {
  vector<vector<string>> result;

  vector<string> tmp;

  for (const string &word : input) {
    if (word == "|") {
      result.push_back(tmp);
      tmp.clear();
    } else {
      tmp.push_back(word);
    }
  }

  result.push_back(tmp);

  return result;
}
