#pragma once

#include <vector>
#include <string>

#include "core/core.h"
#include "engine/core/context.h"
#include "geometry/mesh.h"
#include "texture/texture.h"
#include "engine/gfx/ogl/ogl_shader.h"
#include "engine/core/config.h"

namespace rain::engine
{
    using core::FilePath;

    template<typename T>
    struct DataHandle
    {
        DataHandle(const std::string& _path)
            : path(_path)
            , id(core::uuid::from_name(path.get_path_relative(RAIN_CONFIG->data_root)))
        {
            data = new T();
            data->load(path.get_path_absolute());
        }

        DataHandle()
            : path("")
            , id()
            , data(nullptr) {}

        FilePath path;
        core::uuid id;
        T* data;
    };

    template <typename T>
    struct DataHandleContainer
    {
        void load_data(const std::string& _path)
        {
            DataHandle<T>* handle = new DataHandle<T>(_path);
            datas.push_back(handle);
        }

        std::vector<DataHandle<T>*> datas;
    };


    struct DataSystem
    {
        void load_all_recursive(const std::string& _root);

        Mesh* find_mesh(const FilePath& _path);
        Mesh* find_mesh(const core::uuid _id);

        std::vector<FilePath> paths;

        DataHandleContainer<Mesh> meshes;
        DataHandleContainer<Texture> textures;
        DataHandleContainer<Shader> shaders;
    };

    //struct ShadersInfo
    //{
    //    std::string vertex_path;
    //    std::string fragment_path;
    //    std::string geometry_path;
    //};

}
