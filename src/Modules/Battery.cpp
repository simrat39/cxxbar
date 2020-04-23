#include <fstream>
#include <filesystem>

#include "../Utils/FileUtils.h"

namespace Battery {

    static std::string batteryPath = "/sys/class/power_supply/BAT0";
    static std::string output = "";

    void setBatteryPath() {
        for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator("/sys/class/power_supply/")) {
            if (entry.path().string().find("BAT") != std::string::npos) {
                batteryPath = std::string(entry.path());
                break;
            }
        }
    }

    std::string getBatteryStatus() {
        std::ifstream f_status(batteryPath + "/status");
        const std::string status = FileUtils::readFirstLine(f_status);

        std::ifstream f_capacity(batteryPath + "/capacity");
        const int capacity = std::stoi(FileUtils::readFirstLine(f_capacity));

        std::string icon = "";

        if (status == "Charging" || status == "Unknown") {
            icon = "";
        } else {
            if (capacity >= 80) {
                icon = "";
            } else if (capacity > 60) {
                icon = "";
            } else if (capacity > 40) {
                icon = "";
            } else if (capacity > 20) {
                icon = "";
            } else {
                icon = "";
            }
        }
        output = icon + " " + std::to_string(capacity) + "%";
        return output;
    }

    std::string getOutput() {
        return output;
    }
}
