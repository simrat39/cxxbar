#include <fstream>

#include "../Utils/FileUtils.h"

namespace Network {

    static std::string output = "";

    std::string getNetworkStatus() {
        std::ifstream wifiOperstate("/sys/class/net/wlp7s0/operstate");
        std::ifstream ethOperstate("/sys/class/net/enp8s0/operstate");

        if (FileUtils::doesFileExist(wifiOperstate) && FileUtils::readFirstLine(wifiOperstate) == "up") {
            output = "";
        } else if (FileUtils::doesFileExist(ethOperstate) && FileUtils::readFirstLine(ethOperstate) == "up"){
            output = "";
        } else {
            output = "\uf00d";
        }
        return output;
    }

    std::string getOutput() {
        return output;
    }

}
