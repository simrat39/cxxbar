#include <iostream>
#include <thread>
#include <map>
#include <vector>

#include "Modules/Network.h"
#include "Modules/Battery.h"
#include "Modules/Bspwm.h"
#include "Modules/SimpleDate.h"
#include "Modules/SimpleTime.h"
#include "Modules/DateTime.h"

#include "Utils/ConfigUtils.h"

typedef std::string(*funcPtr)();

// Map of modules <string : getModuleStatus function>
static std::map<std::string , funcPtr> modulesMap;

// Main Properties begin
std::vector<std::string> leftModules;
std::vector<std::string> centerModules;
std::vector<std::string> rightModules;

std::string seperator;

std::string leftPadding;
std::string rightPadding;
// Main Properties end

// Dynamic modules maps
std::map<std::string,DateTime*> dt_map = CustomDateTime::make_date_time_map();

void setPosModulesVector(std::vector<std::string>& posModule, const std::string& property, const std::string& defaultValue) {
    std::string posModuleFromConfig = ConfigUtils::getValue(property,defaultValue);
    std::string module;
    while (posModuleFromConfig.find(",") != std::string::npos) {
        module = posModuleFromConfig.substr(0,posModuleFromConfig.find(","));
        posModuleFromConfig.erase(0,posModuleFromConfig.find(",")+1);
        posModule.push_back(module);
    } 
    module = posModuleFromConfig.substr(0,posModuleFromConfig.size());
    posModule.push_back(module);
}

void setPropertiesFromConfig() {
    // seperator
    std::string seperatorVal = ConfigUtils::getValue("seperator" , "\"  |  \"");
    seperator = seperatorVal.substr(1,seperatorVal.size() - 2);

    // padding
    leftPadding = ConfigUtils::getValue("padding-left","2");
    rightPadding = ConfigUtils::getValue("padding-right","2");

    // network
    Network::setPropertiesFromConfig();

    // left modules vector
    setPosModulesVector(leftModules, std::string("left"), std::string("Date,Time"));
    // center modules vector
    setPosModulesVector(centerModules, std::string("center"), std::string("BSPWM"));
    // right modules vector
    setPosModulesVector(rightModules, std::string("right"), std::string("Network,Battery"));
    
}

void setModuleMap() {
    modulesMap["Battery"] = Battery::getOutput;
    modulesMap["Network"] = Network::getOutput;
    modulesMap["BSPWM"] = Bspwm::getOutput;
    modulesMap["SimpleDate"] = SimpleDate::getOutput;
    modulesMap["SimpleTime"] = SimpleTime::getOutput;
}

void setOutputStringforPosition(std::vector<std::string> posModules, std::string& position) {
    int sizeOfArr = posModules.size();
    if (sizeOfArr) {
        for (int i = 0; i < sizeOfArr; i++) {
            if (modulesMap.find(posModules[i]) != modulesMap.end()) {
                posModules[i] != "" ? position += i == sizeOfArr - 1 ? modulesMap[(posModules)[i]]() : modulesMap[(posModules)[i]]() + seperator : position += "";
            } else if (dt_map.find(posModules[i]) != dt_map.end()) {
                dt_map[(posModules)[i]]->getDate();
                posModules[i] != "" ? position += i == sizeOfArr - 1 ? dt_map[(posModules)[i]]->getOutput() : dt_map[(posModules)[i]]->getOutput() + seperator : position += "";
            }    
        }
    }
}

void setPadding(std::string& amount, std::string& position) {
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

int main() {
    setModuleMap();
    setPropertiesFromConfig();

    std::vector<std::thread> dtthread = CustomDateTime::make_date_time_threads();

    std::thread networkThread(Network::networkLooper); 

    Battery::setBatteryPath();
    std::thread batteryThread(looper,1000,Battery::getBatteryStatus);

    std::thread bspwmThread(Bspwm::BspwmLooper);

    std::thread dateThread(looper , 60000 , SimpleDate::getDate);

    std::thread timeThread(looper, 1000 , SimpleTime::getTime);

    networkThread.join();
    batteryThread.join();   
    bspwmThread.join();
    dateThread.join();
    timeThread.join();
    return 0;
}
