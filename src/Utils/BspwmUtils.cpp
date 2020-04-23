#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>

namespace BspwmUtils {

    int connect() {
        sockaddr_un addr = {AF_UNIX , "/tmp/bspwm_0_0-socket"};
        int fd = socket(AF_UNIX , SOCK_STREAM , 0);
        connect(fd , (sockaddr*)&addr, sizeof(addr));
        return fd;
    }

    void subscribe(int& fd) {
        char buf[sizeof("subscribe")] = "subscribe";
        send(fd , buf , sizeof(buf), 0);
    }

    void readFromSocket(int& fd, std::string* str) {
        char buf[100];
        read(fd , buf , sizeof(buf));
        *str = buf;
    }

    typedef void(*funcPtr)(std::string);

    void continuousRead(int& fd, std::string* str, const int& sleepTime, funcPtr func) {
        char buf[] = "";

        while (true) {
            read(fd , buf , sizeof(char[100]));
            *str = buf;
            func(*str);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        }

    }
    
}
