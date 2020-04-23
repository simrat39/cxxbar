#include "../Utils/BspwmUtils.h"
#include "../Main.h"
#include <thread>

namespace Bspwm {

    static std::string status;

    void BspwmLooper() {
        int fd = BspwmUtils::connect();
        BspwmUtils::subscribe(fd);

        std::string newStatus = status;

        while (true) {
            BspwmUtils::readFromSocket(fd , &status);
            if (status != newStatus) {
                updateOutput();
                newStatus = status;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::string getBspwmStatus() {

        std::string formattedStatus = "";

        int x = 1; // counter
        for (char i : status.substr(0 , status.find("\n"))) {
            if (i == 'O' || i == 'F') {
                formattedStatus += "  %{+u}" + std::to_string(x) + "%{-u}  ";
                x++;
            } else if (i == 'o') {
                formattedStatus += "  " + std::to_string(x) + "  ";
                x++;
            } else if (i == 'f') {
                x++;
            }
        }

        return formattedStatus;
    }

}

