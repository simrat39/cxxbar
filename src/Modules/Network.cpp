#include <fstream>

#include "../Utils/FileUtils.h"

namespace Network {

    std::string getNetworkStatus() {
        std::ifstream wifiOperstate("/sys/class/net/wlp7s0/operstate");
        std::ifstream ethOperstate("/sys/class/net/enp8s0/operstate");

        if (FileUtils::doesFileExist(wifiOperstate) && FileUtils::readFirstLine(wifiOperstate) == "up") {
            return "";
        } else if (FileUtils::doesFileExist(ethOperstate) && FileUtils::readFirstLine(ethOperstate) == "up"){
            return "";
        }

        return "\uf00d";
    }

}
