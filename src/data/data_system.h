#pragma once

#include <vector>

#include "core/id_generator.h"
#include "core/file_path.h"
#include "geometry/mesh.h"
#include "texture/texture.h"

namespace rain
{
    template<typename T>
    struct DataHandle
    {
        T* data;
        unique_id id;
        FilePath path;
    };

    template <typename T>
    struct DataSystem
    {
        void load_data(const std::string& _path)
        {
            DataHandle<T>* handle = new DataHandle<T>();
            handle->id = unique_id();
            handle->path = _path;
            handle->data = new T();
            handle->data->load(_path);
            datas.push_back(handle);
        }

        std::vector<DataHandle<T>*> datas;
    };

    struct DataHandler
    {
        DataSystem<Mesh> meshes;
        DataSystem<Texture> textures;
    };
}