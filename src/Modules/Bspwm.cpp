#include "../Utils/BspwmUtils.h"
#include "../Main.h"
#include "Bspwm.h"
#include <thread>

namespace Bspwm {

    static std::string status;
    static std::string output = "";

    void BspwmLooper() {
        int fd = BspwmUtils::connect();
        BspwmUtils::subscribe(fd);

        std::string newStatus = status;

        while (true) {
            BspwmUtils::readFromSocket(fd , &status);
            if (status != newStatus) {
                getBspwmStatus();
                updateOutput();
                newStatus = status;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::string getBspwmStatus() {
        output = "";
        int x = 1; // counter
        for (char i : status.substr(0 , status.find("\n"))) {
            if (i == 'O' || i == 'F') {
                output += "  %{+u}";
                if (std::stoi((BspwmUtils::getFocusedWS(BspwmUtils::connect()))) == x) {
                    output += std::to_string(x) + "f";
                } else {
                    output += std::to_string(x);
                }
                output += "%{-u}  ";
                x++;
            } else if (i == 'o') {
                output += "  " + std::to_string(x) + "  ";
                x++;
            } else if (i == 'f') {
                x++;
            }
        }

        return output;
    }

    std::string getOutput() {
        return output;
    }

}

