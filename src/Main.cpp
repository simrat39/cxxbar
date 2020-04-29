#include <iostream>
#include <thread>
#include <map>
#include <vector>

#include "Modules/Network.h"
#include "Modules/Battery.h"
#include "Modules/Bspwm.h"
#include "Modules/Date.h"
#include "Modules/Time.h"

#include "Utils/ConfigUtils.h"

typedef std::string(*funcPtr)();

// Map of modules <string : getModuleStatus function>
static std::map<std::string , funcPtr> modulesMap;

// Main Properties begin
std::vector<std::string> leftModules = {"Date","Time"};
std::vector<std::string> centerModules = {"BSPWM"};
std::vector<std::string> rightModules = {"Network","Battery"};

std::string seperator;

std::string leftPadding;
std::string rightPadding;
// Main Properties end

void setPropertiesFromConfig() {
    // seperator
    std::string seperatorVal = ConfigUtils::getValue("seperator" , "\"  |  \"");
    seperator = seperatorVal.substr(1,seperatorVal.size() - 2);

    // padding
    leftPadding = ConfigUtils::getValue("padding-left","2");
    rightPadding = ConfigUtils::getValue("padding-right","2");

    // network
    Network::setPropertiesFromConfig();
}

void setModuleMap() {
    modulesMap["Battery"] = Battery::getOutput;
    modulesMap["Network"] = Network::getOutput;
    modulesMap["BSPWM"] = Bspwm::getOutput;
    modulesMap["Date"] = Date::getOutput;
    modulesMap["Time"] = Time::getOutput;
}

void setupModuleString(std::vector<std::string> posModules, std::string& position) {
    int sizeOfArr = posModules.size();
    if (sizeOfArr) {
        for (int i = 0; i < sizeOfArr; i++) {
            position += i == sizeOfArr - 1 ? modulesMap[(posModules)[i]]() : modulesMap[(posModules)[i]]() + seperator;
        }
    }
}

void updateOutput() {
    std::string left = "%{l}";
    std::string center = "%{c}";
    std::string right = "%{r}";

    // Setup left padding
    for (int i = 0; i < std::stoi(leftPadding); i++) {
        left += " ";
   }

    // Setup left modules
    setupModuleString(leftModules, left);
    // Setup center modules
    setupModuleString(centerModules, center);
    // Setup right modules
    setupModuleString(rightModules, right);
    
    // Setup right padding
    for (int i = 0; i < std::stoi(rightPadding); i++) {
        right += " ";
    }

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

int main() {
    setModuleMap();
    setPropertiesFromConfig();

    std::thread networkThread(Network::networkLooper); 

    Battery::setBatteryPath();
    std::thread batteryThread(looper,1000,Battery::getBatteryStatus);

    std::thread bspwmThread(Bspwm::BspwmLooper);

    std::thread dateThread(looper , 60000 , Date::getDate);

    std::thread timeThread(looper, 1000 , Time::getTime);

    networkThread.join();
    batteryThread.join();   
    bspwmThread.join();
    dateThread.join();
    timeThread.join();
    return 0;
}
