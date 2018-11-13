#pragma once

#include <string>
#include <vector>
#include <utility>

#include <core/types.h>

namespace rain
{
    namespace String
    {
        void replace(std::string & data, const std::string& toSearch, const std::string& replaceStr);
        std::vector<std::string> split(const std::string& _tosplit, const std::string& _delimiter);
        std::pair<std::string, std::string> pair_split(const std::string& _tosplit, const std::string& _delimiter);
    };
}