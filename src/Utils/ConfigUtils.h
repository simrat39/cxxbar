#include <string>
#include <vector>

namespace ConfigUtils {
    
    std::string strip(std::string str);

    std::string getValue(std::string key , std::string defaultVal);

    std::vector<std::string> getEndingNameStartingWith(std::string str);

}