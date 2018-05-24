#include "string_utils.h"



namespace rain
{
    namespace String
    {
        std::vector<std::string> split(const std::string &s, char delim)
        {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }
    }
}