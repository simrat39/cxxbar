#include <string>
#include <vector>

namespace ConfigUtils {
    
    std::string strip(std::string str);

    std::string getValue(const std::string &key , const std::string &defaultVal);

    std::vector<std::string> getEndingNameStartingWith(const std::string &str);
}