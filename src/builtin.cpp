#include "builtin.hpp"
#include "redirect.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <system_error>
#include <unistd.h>
#include <unordered_map>

namespace fs = std::filesystem;

std::unordered_map<std::string, std::unique_ptr<BuiltIn>> &initMap() {

  static std::unordered_map<std::string, std::unique_ptr<BuiltIn>> map = []() {
    std::unordered_map<std::string, std::unique_ptr<BuiltIn>> m;
    m.insert_or_assign("cd", std::make_unique<cd>());
    m.insert_or_assign("echo", std::make_unique<echo>());
    m.insert_or_assign("exit", std::make_unique<myExit>());
    m.insert_or_assign("pwd", std::make_unique<pwd>());
    return m;
  }();
  return map;
}

bool BuiltIn::check_and_execute(std::vector<std::string> &input) {
  if (input.empty())
    return false;

  auto &builtIn_commands = initMap();
  auto it = builtIn_commands.find(input[0]);

  if (it != builtIn_commands.end()) {

    int saved_stdin = dup(0);
    int saved_stdout = dup(1);
    int saved_stderr = dup(2);

    Redirect::check_and_redirect(input);

    it->second->execute(input);

    dup2(saved_stdin, 0);
    dup2(saved_stdout, 1);
    dup2(saved_stderr, 2);

    close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);

    return true;
  }
  return false;
};

void cd::execute(const std::vector<std::string> &input) {
  std::error_code ec;
  auto old_path = fs::current_path();
  std::string target_path;

  if (input.size() == 1 || (input.size() == 2 && input[1] == "~")) {
    auto dest = getenv("HOME");
    if (dest == nullptr)
      return;
    target_path = dest;

  } else if (input.size() == 2 && input[1] == "-") {
    auto dest = getenv("OLDPWD");
    if (dest == nullptr)
      return;
    target_path = dest;

  } else if (input.size() == 2) {
    auto dest = input[1];
    if (input[1].front() == '~') {
      auto front_path = getenv("HOME");
      if (front_path == nullptr)
        return;
      dest = front_path + input[1].substr(1);
    }
    target_path = dest;

  } else {
    std::cout << "USAGE: cd <option> <path> \n";
    return;
  }

  fs::current_path(target_path, ec);
  if (ec) {
    std::cout << "cd: " << ec.message() << '\n';
    return;
  }
  setenv("OLDPWD", old_path.c_str(), 1);
}

void echo::execute(const std::vector<std::string> &input) {
  for (size_t i = 1; i < input.size(); i++) {
    std::cout << input[i] << ' ';
  }

  std::cout << '\n';
}

void myExit::execute(const std::vector<std::string> &input) {
  if (input.size() > 2)
    return;

  if (input.size() == 1) {
    exit(EXIT_SUCCESS);
  } else {
    try {
      int err_status = std::stoi(input[1]);
      if (err_status > 255 || err_status < 0) {
        std::cout << "Number out of range. Option must be a number in range of "
                     "0 to 255\n";
        return;
      }

      exit(err_status);

    } catch (const std::invalid_argument &e) {
      std::cout
          << "Invalid argument. Option must be a number in range of 0 to 255\n";
      return;
    } catch (const std::out_of_range &e) {
      std::cout << "Number out of range. Option must be a number in range of 0 "
                   "to 255\n";
      return;
    }
  }
}

void pwd::execute(const std::vector<std::string> &input) {
  (void)input;

  std::error_code ec;

  auto res = fs::current_path(ec);
  if (ec) {
    std::cout << "pwd: " << ec.message() << '\n';
    return;
  }

  std::cout << res.string() << '\n';
}
