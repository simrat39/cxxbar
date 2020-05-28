#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

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
        char buf[1000];
        read(fd , buf , sizeof(buf));
        *str = buf;
    }

    std::string getFocusedWS(int fd) {
      char buf[] = "query\0-D\0-d\0--names";
      send(fd,buf,sizeof(buf),0);
      std::string ret;
      BspwmUtils::readFromSocket(fd, &ret);
      return ret;
    }

    typedef void(*funcPtr)(std::string);
    
}
