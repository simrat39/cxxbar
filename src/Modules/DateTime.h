#include <string>
#include <vector>
#include <thread>
#include <map>

class DateTime {
    private:
        std::string m_Format;
        std::string output;
        std::string m_Name;
        int m_SleepTime;
    public:
        DateTime(const std::string& name,const std::string& format,const int& sleep) {
            m_Name = name;
            m_Format = format;
            m_SleepTime = sleep;
        }

        int getSleepTime();

        std::string getName();

        std::string getDate();

        std::string getOutput();

        std::string test();

};

void CustomLooper(DateTime m);

namespace CustomDateTime {
    std::vector<std::thread> make_date_time_threads();
    std::map<std::string, DateTime*> make_date_time_map();
}
