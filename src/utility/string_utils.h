#pragma once


#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

namespace rain
{
    namespace String
    {
        std::unordered_map<std::string, std::string> GetArguments(int argc, char** argv);

        std::vector<std::string> split(const std::string &s, char delim);
        template<typename Out>
        void split(const std::string &s, char delim, Out result)
        {
            std::stringstream ss(s);
            std::string item;
            while (std::getline(ss, item, delim))
            {
                *(result++) = item;
            }
        }
    }
}