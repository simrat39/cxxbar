#include <iostream>
#include <thread>
#include <map>

#include "Modules/Network.h"
#include "Modules/Battery.h"
#include "Modules/Bspwm.h"

typedef std::string(*funcPtr)();

// Map of modules <string : getModuleStatus function>
static std::map<std::string , funcPtr> modulesMap;

std::string leftModules[] = {"BSPWM"};
std::string centerModules[] = {};
std::string rightModules[] = {"Network","Battery"};

void setModuleMap() {
    modulesMap["Battery"] = Battery::getBatteryStatus;
    modulesMap["Network"] = Network::getNetworkStatus;
    modulesMap["BSPWM"] = Bspwm::getBspwmStatus;
}

void updateOutput() {
    std::string left = "%{l}";
    std::string center = "%{c}";
    std::string right = "%{r}";

    std::string seperator = "  |  ";

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

    std::thread networkThread(looper,1000,Network::getNetworkStatus); 

    Battery::setBatteryPath();
    std::thread batteryThread(looper,1000,Battery::getBatteryStatus);

    std::thread bspwmThread(Bspwm::BspwmLooper);

    networkThread.join();
    batteryThread.join();   
    bspwmThread.join();
    return 0;
}
