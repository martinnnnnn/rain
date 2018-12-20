#pragma once

#include <chrono>
#include <string>

#include "types.h"


namespace rain
{

    struct ConsoleProfiler
    {
        ConsoleProfiler(const std::string& _message);

        ~ConsoleProfiler();

        std::string message;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;

    };
}