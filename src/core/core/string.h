#pragma once

#include <string>
#include <vector>
#include <utility>

#include "core/types.h"

namespace rain::core
{
    namespace String
    {
        void replace(std::string & data, const std::string& toSearch, const std::string& replaceStr);
        std::vector<std::string> split(const std::string& _tosplit, const std::string& _delimiter);
        std::pair<std::string, std::string> pair_split(const std::string& _tosplit, const std::string& _delimiter);

        void print_to_buffer(char* _dest, size_t _destSize, const char* _format, ...);
        void print_to_buffer_va(char* _dest, size_t _destSize, const char* _format, va_list args);
    };
}