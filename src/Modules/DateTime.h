#include <string>
#include <vector>
#include <thread>
#include <map>

class DateTime {
    private:
        const char* m_Format;
    public:
        std::string output;
        std::string m_Name;
        int m_SleepTime;
        DateTime(std::string name, int sleep) {
            m_Name = name;
            m_SleepTime = sleep;
        }

        std::string getDate();

        std::string getOutput();

        std::string test();

};

void CustomLooper(DateTime m);

namespace CustomDateTime {
    std::vector<DateTime> makeModuleVecFromConfig();
    std::map<std::string, DateTime> make_date_time_map();
    std::vector<std::thread> prepareThreads();
}

