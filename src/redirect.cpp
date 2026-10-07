#include "redirect.hpp"
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <unordered_map>

std::unordered_map<std::string, std::unique_ptr<Redirect>> &redOpt() {
  static std::unordered_map<std::string, std::unique_ptr<Redirect>> map = []() {
    std::unordered_map<std::string, std::unique_ptr<Redirect>> m;
    m.insert_or_assign(">", std::make_unique<ow>());
    m.insert_or_assign(">>", std::make_unique<app>());
    m.insert_or_assign("<", std::make_unique<red>());
    m.insert_or_assign("2>", std::make_unique<redErr>());
    m.insert_or_assign("&>", std::make_unique<redBoth>());
    return m;
  }();

  return map;
}

bool Redirect::check_and_redirect(std::vector<std::string> &input) {
  if (input.empty())
    return false;

  auto &redirection_options = redOpt();

  for (int i = (int)input.size() - 1; i >= 0; i--) {

    auto it = redirection_options.find(input[i]);

    if (it != redirection_options.end()) {
      if (i + 1 < (int)input.size()) {

        if (!it->second->bend_pipe(input[i + 1])) {
          return false;
        };

        input.erase(input.begin() + i, input.begin() + i + 2);
      } else {
        std::cout << "rash: syntax error near unexpected token `newline'\n";
        return false;
      }
    }
  }

  return true;
}

bool ow::bend_pipe(const std::string dest) {
  int fd = open(dest.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd < 0) {
    std::cerr << "Error with opening the file " << dest << '\n';
    return false;
  }

  if (dup2(fd, 1) < 0) {
    std::cerr << "Failed to redirect the standard output \n";
    close(fd);
    return false;
  };

  close(fd);
  return true;
};

bool app::bend_pipe(const std::string dest) {
  int fd = open(dest.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
  if (fd < 0) {
    std::cerr << "Error with opening the file " << dest << '\n';
    return false;
  }

  if (dup2(fd, 1) < 0) {
    std::cerr << "Failed to redirect the standard output \n";
    close(fd);
    return false;
  };

  close(fd);
  return true;
};

bool red::bend_pipe(const std::string dest) {
  int fd = open(dest.c_str(), O_RDONLY);

  if (fd < 0) {
    std::cerr << "File not found or it does not exists " << dest << '\n';
    return false;
  }

  if (dup2(fd, 0) < 0) {
    std::cerr << "Failed to redirect the standard input \n";
    close(fd);
    return false;
  };

  close(fd);
  return true;
};

bool redErr::bend_pipe(const std::string dest) {
  int fd = open(dest.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd < 0) {
    std::cerr << "Error with opening the file " << dest << '\n';
    return false;
  }

  if (dup2(fd, 2) < 0) {
    std::cerr << "Failed to redirect the standard error \n";
    close(fd);
    return false;
  };

  close(fd);
  return true;
};

bool redBoth::bend_pipe(const std::string dest) {
  int fd = open(dest.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd < 0) {
    std::cerr << "Error with opening the file " << dest << '\n';
    return false;
  }

  if (dup2(fd, 1) < 0) {
    std::cerr << "Failed to redirect the standard output \n";
    close(fd);
    return false;
  };

  if (dup2(fd, 2) < 0) {
    std::cerr << "Faield to redirect the standard error \n";
    close(fd);
    return false;
  }

  close(fd);
  return true;
};
