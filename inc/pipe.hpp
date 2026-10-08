#include <string>
#include <vector>

class Pipe {
  Pipe() = default;
  ~Pipe() = default;

  static bool check_and_execute_pipe(std::vector<std::string> &input);
};
