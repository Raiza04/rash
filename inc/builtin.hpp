#pragma once

#include <string>
#include <vector>

class cd;
class echo;
class myExit;
class pwd;

class BuiltIn {
public:
  BuiltIn() = default;
  virtual ~BuiltIn() = default;

  static bool check_and_execute(std::vector<std::string> &input);

protected:
  virtual void execute(const std::vector<std::string> &input) = 0;
};

class cd : public BuiltIn {
protected:
  void execute(const std::vector<std::string> &input) override;
};

class echo : public BuiltIn {
protected:
  void execute(const std::vector<std::string> &input) override;
};

class myExit : public BuiltIn {
protected:
  void execute(const std::vector<std::string> &input) override;
};

class pwd : public BuiltIn {
protected:
  void execute(const std::vector<std::string> &input) override;
};
