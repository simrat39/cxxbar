#include <iomanip>
#include <iostream>
#include <string>

namespace SimpleDate {

std::string output = "";

std::string getDate() {
  auto time = std::time(0);
  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time), "%d %b %Y");
  output = oss.str();
  return oss.str();
}

std::string getOutput() { return output; }
} // namespace SimpleDate
