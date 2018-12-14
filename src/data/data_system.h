#pragma once

#include <vector>

#include "core/id_generator.h"
#include "core/file_path.h"
#include "geometry/mesh.h"
#include "texture/texture.h"
#include "gfx/ogl/ogl_shader.h"

namespace rain
{
    template<typename T>
    struct DataHandle
    {
        DataHandle(const std::string& _path)
            : path(_path)
            , id(RAIN_NEW_NAME_ID(path.get_path_relative()))
        {
            data = new T();
            data->load(path.get_path_absolute());
        }

        DataHandle()
            : path("")
            , id()
            , data(nullptr) {}

        FilePath path;
        unique_id id;
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
        Mesh* find_mesh(const unique_id _id);

        std::vector<FilePath> paths;

        DataHandleContainer<Mesh> meshes;
        DataHandleContainer<Texture> textures;
        DataHandleContainer<Shader> shaders;
    };
}

#define RAIN_FIND_DATA_FROM_PATH(path) rain::Application::get().data_system->find_mesh(path) 
#define RAIN_FIND_DATA_FROM_ID(id) rain::Application::get().data_system->find_mesh(id)