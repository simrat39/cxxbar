#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "../Main.h"
#include "../Utils/ConfigUtils.h"
#include "Script.h"

namespace ScriptModule {

void looper(Script &st) {
  st.runCmd(st.m_Script);
  std::string newStatus = "";

  while (st.m_SleepTime != -1) {
    std::string status = st.runCmd(st.m_Script);
    if (status != newStatus) {
      updateOutput();
      newStatus = status;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(st.m_SleepTime));
  }
}

std::vector<Script *> make_script_vector() {
  std::vector<Script *> vec;

  for (std::string &i : ConfigUtils::getEndingNameStartingWith("module-")) {
    if (ConfigUtils::getValue(i + "-type", "None") == "Script") {
      vec.push_back(new Script(
          i,
          ConfigUtils::getValue(i + "-command", "echo \"command not found\""),
          std::stoi(ConfigUtils::getValue(i + "-sleep", "1000")),
          ConfigUtils::getValue(i + "-left-click", ""),
          ConfigUtils::getValue(i + "-right-click", ""),
          ConfigUtils::getValue(i + "-middle-click", ""),
          ConfigUtils::getValue(i + "-scroll-up", ""),
          ConfigUtils::getValue(i + "-scroll-down", ""),
          ConfigUtils::getValue(i + "-underline-color", ""),
          ConfigUtils::getValue(i + "-background-color", ""),
          ConfigUtils::getValue(i + "-foreground-color", ""),
          ConfigUtils::getValueWithQuotes(i + "-format", "<output>")));
    }
  }
  return vec;
}

std::vector<std::thread> make_script_threads(std::vector<Script *> &sc_vec) {
  std::vector<std::thread> vec;
  for (Script *i : sc_vec) {
    vec.push_back(std::thread(ScriptModule::looper, std::ref(*i)));
  }
  return vec;
}

std::map<std::string, Script *> make_script_map(std::vector<Script *> &sc_vec) {
  std::map<std::string, Script *> map;
  for (Script *i : sc_vec) {
    map[i->getName()] = i;
  }
  return map;
}

void format_output_str_pre(std::string &pre, const std::string &format_this) {
  int index = format_this.find("<output>");
  if (index == std::string::npos)
    return;
  pre += format_this.substr(0, index);
}

void format_output_str_post(std::string &post, const std::string &format_this) {
  int index = format_this.find("<output>");
  if (index == std::string::npos)
    return;
  post = format_this.substr(index + 8);
}

} // namespace ScriptModule

std::string Script::getName() { return m_Name; }

std::string Script::getOutput() { return pre + output + post; }

std::string Script::runCmd(const std::string &cmd) {
  std::string result;
  std::array<char, 128> buffer;

  auto pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    return "";

  while (!feof(pipe)) {
    if (fgets(buffer.data(), 128, pipe) != nullptr) {
      result = buffer.data();
      result = result.substr(0, result.find("\n"));
    }
    this->output = result;
    if (m_SleepTime == -1) {
      result = "";
      updateOutput();
    }
  }

  auto rc = pclose(pipe);
  return result;
}
