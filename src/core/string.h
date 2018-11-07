#pragma once

#include <string>


namespace rain
{
    class String
    {
    public:
        static void replace(std::string & data, const std::string& toSearch, const std::string& replaceStr);
    };
}