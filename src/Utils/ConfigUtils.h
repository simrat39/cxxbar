#include <string>
#include <vector>

namespace ConfigUtils {
    
    std::string strip(std::string str);

    std::string getValue(const std::string &key , const std::string &defaultVal);

    std::vector<std::string> getEndingNameStartingWith(const std::string &str);

    void parsePrePostStrings(const std::string &left_click, const std::string &right_click,
            const std::string &middle_click, const std::string &scroll_up, const std::string &scroll_down,
            std::string &pre, std::string &post, std::string& underline_color, std::string &background_color, std::string& foreground_color);
}
