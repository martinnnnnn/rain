#include "profiler.h"

namespace rain::core
{
    ConsoleProfiler::ConsoleProfiler(void(*_printOutput)(const double), const std::string& _message)
		: message(_message)
		, print_output(_printOutput)
    {
        start = std::chrono::system_clock::now();
    }

    ConsoleProfiler::~ConsoleProfiler()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
		print_output(elapsed_seconds.count() * 1000);
    }
}

