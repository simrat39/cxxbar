#include <fstream>
#include <string>
#include <iostream>

namespace ConfigUtils {

    std::string strip(std::string str) {
        while (str[0] == ' ') {
            (str).erase(0,1);
        }
        while (str[str.size() - 1] == ' ') {
            (str).erase((str.size() - 1),1);
        }
        return str;
    }

    std::string getValue(std::string key , std::string defaultVal) {
        std::ifstream file(std::string(getenv("HOME")) + std::string("/.config/cxxbar/conf"));
        std::string line;
        while (getline(file,line)) {
            std::string l_key = ConfigUtils::strip(line.substr(0,line.find("=")));
            std::string l_value = ConfigUtils::strip(line.substr(line.find("=") + 1,sizeof(line)));
            if (l_key == key) {
                file.close();
                return l_value;
            }
        }
        file.close();        
        return defaultVal;
    }

}