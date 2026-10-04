#include <cstring>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

int main() {

  while (true) {
    std::cout << "rash>" << std::flush;

    std::vector<std::string> testing;

    std::string testString;

    std::getline(std::cin, testString);

    if (std::cin.eof()) {
      break;
    }
    if (testString == "exit") {
      break;
    }

    std::istringstream stream(testString);

    std::string wort;

    while (stream >> wort) {
      std::cout << wort << '\n';
    }
  }

  return 0;
}
