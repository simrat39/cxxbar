#include <string>

namespace BspwmUtils {

int connect();

void subscribe(int &fd);

void readFromSocket(int &fd, std::string *str);

std::string getFocusedWS(int fd);

typedef void (*funcPtr)(std::string);

} // namespace BspwmUtils
