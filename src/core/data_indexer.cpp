#include "data_indexer.h"



namespace rain
{
    DataIndexer::DataIndexer(std::string _path)
    {
        if (!file.open(_path))
        {
            return;
        }

        std::vector<std::string> lines = String::split(file.read(), "\n");
        for (u32 i = 0; i < lines.size(); ++i)
        {
            auto dataPair = String::pair_split(lines[i], "=");
            if (dataPair.first == "indexer")
            {
                auto indexer_data = String::pair_split(dataPair.second, "=");
                indexers.push_back(new DataIndexer(RAIN_DATA.root + indexer_data.second));
            }
            else
            {
                data_map[dataPair.first] = dataPair.second;
            }
        }
        file.close();
    }

    bool DataIndexer::find(const Key& _key, Value& _value)
    {
        if (data_map.find(_key) != data_map.end())
        {
            _value = data_map.at(_key);
            return true;
        }
        return false;
    }

    DataIndexer* DataIndexer::find(const std::string& _indexer)
    {
        for (u32 i = 0; i < indexers.size(); ++i)
        {
            if (indexers[i]->file.get_name() == _indexer)
            {
                return indexers[i];
            }
        }

        return nullptr;
    }
}