#pragma once

#include <vector>
#include <string>


#include "core/singleton.h"
#include "core/file.h"

#include <string.h>
#include <Windows.h>

namespace rain
{

    class Data : public Singleton<Data>
    {


    public:
        void init()
        {
            std::string path = File::GetExePath() + "/config.rain";

            File config;
            if (config.open(path))
            {
                root_path = config.read();
            }
        }

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