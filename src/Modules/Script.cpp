#include <string>
#include <thread>
#include <chrono>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <functional>
#include <iostream>

#include "../Main.h"
#include "Script.h"
#include "../Utils/ConfigUtils.h"

namespace ScriptModule {
    
    void looper(Script& st) {
        st.runCmd(st.m_Script);
        std::string newStatus = "";

        while (st.m_SleepTime != -1) {
            std::string status = st.runCmd(st.m_Script); 
            if (status != newStatus) {
                updateOutput();
                newStatus = status;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(st.m_SleepTime));
        }
    }

    std::vector<Script*> make_script_vector() {
        std::vector<Script*> vec;

        for (std::string& i : ConfigUtils::getEndingNameStartingWith("module-")) {
            if (ConfigUtils::getValue(i + "-type", "None") == "Script") {
                vec.push_back(new Script(i, ConfigUtils::getValue(i + "-command", "echo \"command not found\""), std::stoi(ConfigUtils::getValue(i + "-sleep", "1000"))));
            }
        }
        return vec;
    }

    std::vector<std::thread> make_script_threads(std::vector<Script*> &sc_vec) {
        std::vector<std::thread> vec;
        for (Script *i : sc_vec) {
            vec.push_back(std::thread(ScriptModule::looper, std::ref(*i)));
        }
        return vec;
    }

    std::map<std::string, Script*> make_script_map(std::vector<Script*> &sc_vec) {
        std::map<std::string, Script*> map;
        for (Script *i : sc_vec) {
            map[i->getName()] = i;
        }
        return map;
    }
    
}
    
std::string Script::getName() { return m_Name; }

std::string Script::getOutput() { return output.substr(0,output.find("\n")); }

std::string Script::runCmd(const std::string &cmd) {
    std::string result;
    std::array<char, 128> buffer;

    auto pipe = popen(cmd.c_str(), "r");

    while (!feof(pipe)) {
        if (m_SleepTime == -1)
            result="";
        if (fgets(buffer.data(), 128, pipe) != nullptr) {
            result += buffer.data();
        }
        this->output = result;
        std::clog << output << "\n";
        updateOutput();
    }

    auto rc = pclose(pipe);
    return result;
}
