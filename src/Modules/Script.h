#include <thread>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <map>
#include "../Main.h"
#include "../Utils/ConfigUtils.h"

class Script {
    private:
        std::string m_Name;
        // Clicking events , 1 - 5
        std::string m_left_click, m_right_click, m_middle_click, m_scroll_up, m_scroll_down;
        // Colors
        std::string m_underline_color, m_background_color, m_text_color;
        // Output strings
        // Main output string
        std::string output;
        // Prefix to output
        std::string pre;
        // Suffix to output
        std::string post;
    public:
        std::string m_Script;
        int m_SleepTime;
        
        inline Script(const std::string &name, const std::string &script, const int &sleepTime,
                const std::string& left_click, const std::string& right_click, const std::string& middle_click,
                const std::string& scroll_up, const std::string& scroll_down, const std::string& underline_color,
                const std::string& background_color, const std::string& foreground_color)
            : m_Name(name), m_Script(script), m_SleepTime(sleepTime), m_left_click(left_click),
                m_right_click(right_click), m_middle_click(middle_click), m_scroll_up(scroll_up),
                m_scroll_down(scroll_down), m_underline_color(underline_color), m_background_color(background_color), m_text_color(foreground_color) {
                
            ConfigUtils::parsePrePostStrings(m_left_click, m_right_click, m_middle_click, m_scroll_up, m_scroll_down,
                                                pre, post, m_underline_color, m_background_color, m_text_color);
        }

        std::string getName(); 

        std::string getOutput();

        std::string runCmd(const std::string &cmd); 
};

namespace ScriptModule {
    void looper(Script &sc);

    std::vector<Script*> make_script_vector();

    std::vector<std::thread> make_script_threads(std::vector<Script*> &sc_vec);

    std::map<std::string, Script*> make_script_map(std::vector<Script*> &sc_vec);
}
