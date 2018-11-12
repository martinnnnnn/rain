#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <map>
#include <cassert>

#include "core/singleton.h"
#include "core/file.h"
#include "core/string.h"
#include "win32/win32_application.h"


namespace rain
{
    struct DataIndexer
    {
        using Key = std::string;
        using Value = std::string;
        using DataMap = std::map<Key, Value>;

        DataIndexer(std::string _path);

        bool find(const Key& _key, Value& _value);
        Value find(const Key& _indexer);
        DataIndexer* find_indexer(const std::string& _indexer);

        File file;
        DataMap data_map;
        std::vector<DataIndexer*> indexers;
    };


    class Data
    {
    public:
        void init(const std::string& _path)
        {
            config = new DataIndexer(_path);
            bool dataRootFound = config->find("data_path", root);
            assert(dataRootFound);
            root = File::get_directory(root);
        }
        std::string root;
        DataIndexer* config;
    };
}

#define RAIN_DATA rain::Application::get().data
