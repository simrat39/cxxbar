#include <iostream>
#include <thread>
#include <map>

#include "Modules/Network.h"
#include "Modules/Battery.h"
#include "Modules/Bspwm.h"
#include "Modules/Date.h"
#include "Modules/Time.h"

#include "Utils/ConfigUtils.h"

typedef std::string(*funcPtr)();

// Map of modules <string : getModuleStatus function>
static std::map<std::string , funcPtr> modulesMap;

std::string leftModules[] = {"Date","Time"};
std::string centerModules[] = {"BSPWM"};
std::string rightModules[] = {"Network","Battery"};

std::string seperator;

void setPropertiesFromConfig() {
    // seperator
    std::string seperatorVal = ConfigUtils::getValue("seperator" , "\"  |  \"");
    seperator = seperatorVal.substr(1,seperatorVal.size() - 2);
}

void setModuleMap() {
    modulesMap["Battery"] = Battery::getOutput;
    modulesMap["Network"] = Network::getOutput;
    modulesMap["BSPWM"] = Bspwm::getOutput;
    modulesMap["Date"] = Date::getOutput;
    modulesMap["Time"] = Time::getOutput;
}

void updateOutput() {
    const char* leftPadding = " ";
    const char* rightPadding = " ";

    std::string left = std::string("%{l}") + leftPadding;
    std::string center = "%{c}";
    std::string right = "%{r}";

    int sizeOfArr = sizeof(leftModules)/24;
    if (sizeOfArr) {
        for (int i = 0; i < sizeOfArr; i++) {
            left += i == sizeOfArr - 1 ? modulesMap[leftModules[i]]() : modulesMap[leftModules[i]]() + seperator;
        }
    }
    

    sizeOfArr = sizeof(centerModules)/24;
    if (sizeOfArr) {
        for (int i = 0; i < sizeOfArr; i++) {
            center += i == sizeOfArr - 1 ? modulesMap[centerModules[i]]() : modulesMap[centerModules[i]]() + seperator;
        }
    }
    

    sizeOfArr = sizeof(rightModules)/24;
    if (sizeOfArr) {
        for (int i = 0; i < sizeOfArr; i++) {
            right += i == sizeOfArr - 1 ? modulesMap[rightModules[i]]() : modulesMap[rightModules[i]]() + seperator;
        }
    }
    
    right += rightPadding;

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
