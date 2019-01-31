#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>

namespace rain::core
{
    namespace string
    {
        void replace(std::string & data, const std::string& toSearch, const std::string& replaceStr);
        std::vector<std::string> split(const std::string& _tosplit, const std::string& _delimiter);
        void pair_split(const std::string& _tosplit, const std::string& _delimiter, std::string& first, std::string& second);

        inline void to_upper(std::string& _str)
        {
            std::transform(_str.begin(), _str.end(), _str.begin(), ::toupper);
        }

        inline void to_lower(std::string& _str)
        {
            std::transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
        }



        void print_to_buffer(char* _dest, size_t _destSize, const char* _format, ...);
        void print_to_buffer_va(char* _dest, size_t _destSize, const char* _format, va_list args);

    };
}