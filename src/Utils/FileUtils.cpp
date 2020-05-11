#include <iostream>
#include <fstream>
#include <string>

namespace FileUtils {

    std::string readFirstLine(std::ifstream& file) {
        std::string firstLine;
        getline(file , firstLine);
        return firstLine;
    }

    bool doesFileExist(const std::ifstream& file) {
        return (bool)file;
    }

}