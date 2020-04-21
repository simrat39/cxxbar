#include <string>

namespace FileUtils {
    std::string readFirstLine(std::ifstream& file);

    bool doesFileExist(std::ifstream& file);
}