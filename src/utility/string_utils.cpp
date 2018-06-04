#include "string_utils.h"



namespace rain
{
    namespace String
    {
        std::unordered_map<std::string, std::string> GetArguments(int _argc, char** _argv)
        {
            std::unordered_map<std::string, std::string> args;
            for (int i = 0; i < _argc; i++)
            {
                std::vector<std::string> first = String::split(_argv[i], ';');
                for (size_t j = 0; j < first.size(); j++)
                {
                    std::vector<std::string> second = String::split(first[j], '=');
                    if (second.size() > 1)
                    {
                        args[second[0]] = second[1];
                    }
                }
            }
            return args;
        }

        std::vector<std::string> split(const std::string &s, char delim)
        {
            std::vector<std::string> elems;
            split(s, delim, std::back_inserter(elems));
            return elems;
        }
    }
}