#include <string>
#include <iomanip>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>

#include "DateTime.h"

#include "../Main.h"
#include "../Utils/ConfigUtils.h"

std::string DateTime::getDate(){
    auto time = std::time(0);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), m_Format.c_str());
    this->output = oss.str();
    return oss.str();
}

std::string DateTime::getOutput() {
    return output;
}

std::string DateTime::getName() {
    return m_Name;
}


int DateTime::getSleepTime() {
    return m_SleepTime;
}

void CustomLooper(DateTime tm) {
    std::string newStatus = "";
    while (true) {
        std::string status = tm.getDate();

        if (status != newStatus) {
            updateOutput();
            newStatus = status;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(tm.getSleepTime()));
    }
}

namespace CustomDateTime {
    std::vector<DateTime*> make_date_time_vector() {
        std::vector<DateTime*> dtVec;
        std::vector<std::string> names = ConfigUtils::getEndingNameStartingWith("cus_date_time-");
        for (std::string& name : names) {
            dtVec.push_back(new DateTime(name,ConfigUtils::getValue("cus_date_time-" + name, "%I:%M %p"),500));
        }
        return dtVec;
    }

    std::vector<std::thread> make_date_time_threads(const std::vector<DateTime*>& dtVec) {
        std::vector<std::thread> dtThreadVec;
        for (DateTime* dt : dtVec) {
            dtThreadVec.push_back(std::thread(CustomLooper,*dt));
        }
        return dtThreadVec;
    }

    std::map<std::string, DateTime*> make_date_time_map(const std::vector<DateTime*>& dtVec) {
        std::map<std::string, DateTime*> map;
        for (DateTime* dt : dtVec) {
            map[dt->getName()] = dt;
        }
        return map;
    }
}


