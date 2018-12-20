#include "profiler.h"

#include "core/logger.h"

namespace rain
{
    ConsoleProfiler::ConsoleProfiler(const std::string& _message) : message(_message)
    {
        start = std::chrono::system_clock::now();
    }

    ConsoleProfiler::~ConsoleProfiler()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        RAIN_LOG_PROFILE((message + " %f").c_str(), elapsed_seconds.count() * 1000);
    }
}

