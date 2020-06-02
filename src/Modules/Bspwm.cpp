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
        }
    }

    std::string getBspwmStatus() {
        output = "";
        int x = 1; // counter
        for (char i : status.substr(0 , status.find("\n"))) {
            if (i == 'O' || i == 'F') {
                output += "  %{+u}";
                output += std::to_string(x);
                /* if (std::stoi((BspwmUtils::getFocusedWS(BspwmUtils::connect()))) == x) */
                /*     output += "f"; */
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

