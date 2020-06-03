#include <any>
#include <iostream>
#include <map>
#include <thread>
#include <vector>

#include "Modules/Battery.h"
#include "Modules/Bspwm.h"
#include "Modules/Network.h"
#include "Modules/Script.h"
#include "Modules/SimpleDate.h"
#include "Modules/SimpleTime.h"

#include "Utils/ConfigUtils.h"

typedef std::string (*funcPtr)();

// Map of modules <string : getModuleStatus function>
static std::map<std::string, funcPtr> modulesMap;

// Main Properties begin
std::vector<std::string> leftModules;
std::vector<std::string> centerModules;
std::vector<std::string> rightModules;

std::string seperator;

std::string leftPadding;
std::string rightPadding;
// Main Properties end

// Custom Modules
std::vector<Script *> script_module_vector = ScriptModule::make_script_vector();
std::map<std::string, Script *> script_module_map =
    ScriptModule::make_script_map(script_module_vector);

void setPosModulesVector(std::vector<std::string> &posModule,
                         const std::string &property,
                         const std::string &defaultValue) {
  std::string posModuleFromConfig =
      ConfigUtils::getValue(property, defaultValue);
  std::string module;
  while (posModuleFromConfig.find(",") != std::string::npos) {
    module = posModuleFromConfig.substr(0, posModuleFromConfig.find(","));
    posModuleFromConfig.erase(0, posModuleFromConfig.find(",") + 1);
    posModule.push_back(module);
  }
  module = posModuleFromConfig.substr(0, posModuleFromConfig.size());
  posModule.push_back(module);
}

void setPropertiesFromConfig() {
  // seperator
  std::string seperatorVal = ConfigUtils::getValue("seperator", "\"  |  \"");
  seperator = seperatorVal.substr(1, seperatorVal.size() - 2);

  // padding
  leftPadding = ConfigUtils::getValue("padding-left", "2");
  rightPadding = ConfigUtils::getValue("padding-right", "2");

  // network
  Network::setPropertiesFromConfig();

  // left modules vector
  setPosModulesVector(leftModules, std::string("left"),
                      std::string("Date,Time"));
  // center modules vector
  setPosModulesVector(centerModules, std::string("center"),
                      std::string("BSPWM"));
  // right modules vector
  setPosModulesVector(rightModules, std::string("right"),
                      std::string("Network,Battery"));
}

void setModuleMap() {
  modulesMap["Battery"] = Battery::getOutput;
  modulesMap["Network"] = Network::getOutput;
  modulesMap["BSPWM"] = Bspwm::getOutput;
  modulesMap["SimpleDate"] = SimpleDate::getOutput;
  modulesMap["SimpleTime"] = SimpleTime::getOutput;
}

void setOutputStringforPosition(std::vector<std::string> posModules,
                                std::string &position) {
  int sizeOfArr = posModules.size();
  if (sizeOfArr) {
    for (int i = 0; i < sizeOfArr; i++) {
      if (modulesMap.find(posModules[i]) != modulesMap.end()) {
        posModules[i] != ""
            ? position += i == sizeOfArr - 1
                              ? modulesMap[(posModules)[i]]()
                              : modulesMap[(posModules)[i]]() + seperator
            : position += "";
      } else if (script_module_map.find(posModules[i]) !=
                 script_module_map.end()) {
        posModules[i] != ""
            ? position +=
              i == sizeOfArr - 1
                  ? script_module_map[(posModules)[i]]->getOutput()
                  : script_module_map[(posModules)[i]]->getOutput() + seperator
            : position += "";
      }
    }
  }
}

void setPadding(const std::string &amount, std::string &position) {
  for (int i = 0; i < std::stoi(amount); i++) {
    position += " ";
  }
}

void updateOutput() {
  std::string left = "%{l}";
  std::string center = "%{c}";
  std::string right = "%{r}";

  // Setup left padding
  setPadding(leftPadding, left);

  // Setup left modules
  setOutputStringforPosition(leftModules, left);
  // Setup center modules
  setOutputStringforPosition(centerModules, center);
  // Setup right modules
  setOutputStringforPosition(rightModules, right);

  // Setup right padding
  setPadding(rightPadding, right);

  std::cout << left + center + right << std::endl;
}

void looper(int sleepTime, funcPtr func) {
  std::string newStatus = "";
  while (true) {
    std::string status = func();

    if (status != newStatus) {
      updateOutput();
      newStatus = status;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
  }
}

void dummy() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000000));
  }
}

int main() {

  setModuleMap();
  setPropertiesFromConfig();

  std::string allModules;

  for (std::string i : leftModules) {
    allModules += i + " ";
  }
  for (std::string i : centerModules) {
    allModules += i + " ";
  }
  for (std::string i : rightModules) {
    allModules += i + " ";
  }

  std::thread networkThread, batteryThread, bspwmThread, dateThread, timeThread,
      simThread;
  std::vector<std::thread> script_module_threads_vector;
  if (script_module_vector.size()) {
    script_module_threads_vector =
        ScriptModule::make_script_threads(script_module_vector);
  }

  if (allModules.find("Network") != std::string::npos) {
    networkThread = std::thread(Network::networkLooper);
  }
  if (allModules.find("Battery") != std::string::npos) {
    Battery::setBatteryPath();
    batteryThread = std::thread(looper, 1000, Battery::getBatteryStatus);
  }
  if (allModules.find("BSPWM") != std::string::npos) {
    bspwmThread = std::thread(Bspwm::BspwmLooper);
  }
  if (allModules.find("SimpleDate") != std::string::npos) {
    dateThread = std::thread(looper, 60000, SimpleDate::getDate);
  }
  if (allModules.find("SimpleTime") != std::string::npos) {
    timeThread = std::thread(looper, 1000, SimpleTime::getTime);
  }

  std::thread dummyThread(dummy);
  dummyThread.join();
  return 0;
}
