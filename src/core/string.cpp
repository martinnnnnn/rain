#include "string.h"



namespace rain
{
    void String::replace(std::string & data, const std::string& toSearch, const std::string& replaceStr)
    {
        size_t pos = data.find(toSearch);

        while (pos != std::string::npos)
        {
            data.replace(pos, toSearch.size(), replaceStr);
            pos = data.find(toSearch, pos + toSearch.size());
        }
    }
}