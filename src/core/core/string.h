#pragma once

#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <algorithm> 
#include <cctype>
#include <locale>

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

        inline void ltrim(std::string& str)
        {
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch)
            {
                return !std::isspace(ch);
            }));
        }

        inline void rtrim(std::string& str)
        {
            str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch)
            {
                return !std::isspace(ch);
            }).base(), str.end());
        }

        inline void trim(std::string& str)
        {
            ltrim(str);
            rtrim(str);
        }

        inline std::string ltrim(const char* cstr)
        {
            std::string str = cstr;
            ltrim(str);
            return str;
        }

        inline std::string rtrim(const char* cstr)
        {
            std::string str = cstr;
            rtrim(str);
            return str;
        }

        inline std::string trim(const char* cstr)
        {
            std::string str = cstr;
            trim(str);
            return str;
        }

            void print_to_buffer(char* _dest, size_t _destSize, const char* _format, ...);
        void print_to_buffer_va(char* _dest, size_t _destSize, const char* _format, va_list args);

    };
}