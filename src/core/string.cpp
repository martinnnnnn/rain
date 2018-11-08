#include "string.h"

#include <cassert>


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

    std::vector<std::string> String::split(const std::string& _tosplit, const std::string& _delimiter)
    {
        std::vector<std::string> splited;

        u32 start = 0U;
        std::string::size_type end = _tosplit.find(_delimiter);

        while (end != std::string::npos)
        {
            splited.push_back(_tosplit.substr(start, end - start));
            start = end + _delimiter.length();
            end = _tosplit.find(_delimiter, start);
        }

        splited.push_back(_tosplit.substr(start, end));
        return splited;
    }

    std::pair<std::string, std::string> String::pair_split(const std::string& _tosplit, const std::string& _delimiter)
    {
        std::vector<std::string> splited;

        u32 start = 0U;
        std::string::size_type delimPos = _tosplit.find(_delimiter);
        assert(delimPos != std::string::npos);

        std::string first = _tosplit.substr(start, delimPos - start);
        std::string second = _tosplit.substr(delimPos + 1, (second.length() - 1) - delimPos);

        return std::make_pair(first, second);
    }
}