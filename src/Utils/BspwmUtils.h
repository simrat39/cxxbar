#include <string>

namespace BspwmUtils {

    int connect();

    void subscribe(int& fd);

    void readFromSocket(int& fd, std::string* str);

    typedef void(*funcPtr)(std::string);

}
