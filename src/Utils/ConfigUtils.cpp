#include <fstream>
#include <string>
#include <iostream>
#include <vector>

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

    std::string getValue(const std::string &key , const std::string &defaultVal) {
        std::ifstream file(std::string(getenv("HOME")) + std::string("/.config/cxxbar/conf"));
        std::string line;
        while (getline(file,line)) {
            std::string l_key = ConfigUtils::strip(line.substr(0,line.find("=")));
            std::string l_value = ConfigUtils::strip(line.substr(line.find("=") + 1));
            if (l_key == key) {
                file.close();
                return l_value;
            }
        }
        file.close(); 
        return defaultVal;
    }

    std::vector<std::string> getEndingNameStartingWith(const std::string &str) {
        std::ifstream file(std::string(getenv("HOME")) + std::string("/.config/cxxbar/conf"));
        std::string line;
        std::vector<std::string> vec;
        while (getline(file,line)) {
            std::string l_key = ConfigUtils::strip(line.substr(0,line.find("=")));
            if (l_key.starts_with(str)) {
                l_key.erase(0,str.length());
                vec.push_back(l_key);
            }
        }
        file.close();
        return vec;        
    }

    void parsePrePostStrings(const std::string &left_click,const std::string &right_click,
            const std::string &middle_click,const std::string &scroll_up,const std::string &scroll_down,
            std::string &pre,std::string &post) {
        std::string arr[5] = {left_click,right_click,middle_click,scroll_up,scroll_down};
        pre = "";
        post = "";
        int count = 1;
        for (auto i : arr) {
            if (!i.empty()) {
                pre += "%{A" + std::to_string(count) + ":" + i + ":}";
                post += "%{A}";
            }
            count++;
        }
    }
}
