#include <thread>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <map>
#include "../Main.h"

class Script {
    public:
        std::string m_Name;
        std::string m_Script;
        int m_SleepTime;
        std::string output;
        
        Script(const std::string &name, const std::string &script, const int &sleepTime) : m_Name(name), m_Script(script), m_SleepTime(sleepTime) {}
    
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
