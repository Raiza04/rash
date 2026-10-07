#pragma once

#include <string>
#include <vector>

class ow;      // >
class app;     // >>
class red;     // <
class redErr;  // 2>
class redBoth; // &>

class Redirect {
public:
  Redirect() = default;
  virtual ~Redirect() = default;
  static bool check_and_redirect(std::vector<std::string> &input);

protected:
  virtual bool bend_pipe(const std::string dest) = 0;
};

class ow : public Redirect {
  bool bend_pipe(const std::string dest) override;
};

class app : public Redirect {
  bool bend_pipe(const std::string dest) override;
};

class red : public Redirect {
  bool bend_pipe(const std::string dest) override;
};

class redErr : public Redirect {
  bool bend_pipe(const std::string dest) override;
};

class redBoth : public Redirect {
  bool bend_pipe(const std::string dest) override;
};
