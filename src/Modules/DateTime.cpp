#include <string>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "../Main.h"
#include "../Utils/ConfigUtils.h"
#include "DateTime.h"
#include <map>

std::string DateTime::getDate(){
    auto time = std::time(0);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), m_Format);
    output = oss.str();
    return oss.str();
}

std::string DateTime::getOutput() {
    return output;
}

void CustomLooper(DateTime m) {
    std::string newStatus = "";
    while (true) {
        std::string status = m.getDate();

        if (status != newStatus) {
            updateOutput();
            newStatus = status;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(m.m_SleepTime));
    }
}

std::string DateTime::test() {
    return m_Name;
}

namespace CustomDateTime {

    std::vector<DateTime> makeModuleVecFromConfig() {
        std::vector<DateTime> dt_vector;
        std::vector <std::string> bruh = {"sim","Noob"};
        for (std::string& m : bruh) {
            dt_vector.push_back(DateTime(m, 500));
        }
        return dt_vector;
    }

    std::map<std::string, DateTime>  make_date_time_map() {
        std::map<std::string, DateTime> map;
        for (DateTime dt : makeModuleVecFromConfig()) {
            //map[dt.m_Name] = dt;
        }
        return map;
    }    

    std::vector<std::thread> prepareThreads() {
        std::vector<std::thread> dt_vec_thread;
        for (DateTime dt : makeModuleVecFromConfig()) {
            dt_vec_thread.push_back(std::thread(CustomLooper,dt));
        }
        return dt_vec_thread;
    }
}
