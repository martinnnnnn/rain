#pragma once

#include <chrono>
#include <string>

#include "core/types.h"


namespace rain::core
{

    struct ConsoleProfiler
    {
        ConsoleProfiler(void(*_printOutput)(const double), const std::string& _message);

        ~ConsoleProfiler();

		void(*print_output)(const double _elapsedMs);

        std::string message;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;

    };
}