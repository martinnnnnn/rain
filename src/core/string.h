#pragma once

#include <string>
#include <vector>

#include <core/types.h>

namespace rain
{
    class String
    {
    public:
        static void replace(std::string & data, const std::string& toSearch, const std::string& replaceStr);
        static std::vector<std::string> split(const std::string& _tosplit, const std::string& _delimiter);
    };
}