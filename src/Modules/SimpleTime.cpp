#include <iomanip>
#include <iostream>
#include <string>

namespace SimpleTime {

std::string output = "";

std::string getTime() {
  auto time = std::time(0);
  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time), "%I:%M %p");
  output = oss.str();
  return oss.str();
}

std::string getOutput() { return output; }
} // namespace SimpleTime
