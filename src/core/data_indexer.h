#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <map>


#include "core/singleton.h"
#include "core/file.h"
#include "core/string.h"


namespace rain
{

    class Data : public Singleton<Data>
    {


    public:
        bool init()
        {
            const std::string path = File::GetExePath() + "/config.rain";

            File configFile;
            if (!configFile.open(path))
            {
                return false;
            }
            
            const std::string lines = configFile.read();
            std::vector<std::string> splitedLines = String::split(lines, "\n");
            std::map<std::string, std::string> config;
            for (u32 i = 0; i < splitedLines.size(); ++i)
            {
                std::vector<std::string> line = String::split(splitedLines[i], "=");
                config[line[0]] = line[1];
                if (line[0] == "data_path")
                {
                    root_path = line[1];
                }
            }

            bool data_found = find("data_path", root_path);
            assert(data_found);
        }

        bool find(const std::string& _key, std::string& _value)
        {
            if (config.find(_key) != config.end())
            {
                _value = config.at(_key);
                return true;
            }
            return false;
        }

        std::map<std::string, std::string> config;
        std::string root_path;
    };


    //struct Data
    //{
    //    /*

    //    string path
    //    u64 id


    //    */
    //};


    struct DataIndexer
    {
        std::vector<DataIndexer*> indexers;
        std::vector<Data> data;
    };

}

#define RAIN_DATA rain::Data::Get()