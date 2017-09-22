#ifndef O2_RESULT_MANAGER_H
#define O2_RESULT_MANAGER_H

#include <string>
#include <fstream>
#include <chrono>
namespace O2{
    class ResultManager{
    private:
        std::ofstream output;
        std::chrono::time_point<std::chrono::system_clock> last;
    public:
        ResultManager(const std::string& fileName = "results.csv");
        void addTimeFrame(uint16_t frame);

        ~ResultManager();
    };
}

#endif