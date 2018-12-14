#include "data_system.h"

#include <filesystem>

#include "core/logger.h"

namespace rain
{
    namespace fs = std::filesystem;

    void DataSystem::load_all_recursive(const std::string& _root)
    {
        for (auto& p : fs::recursive_directory_iterator(_root))
        {
            std::string path = p.path().string();
            String::replace(path, "\\", "/");
            FilePath filepath(path);
            if (!p.is_directory())
            {
                paths.push_back(filepath);
                std::string ext = filepath.get_extention();
                RAIN_LOG("%s   -> (%s)", filepath.get_path_relative().c_str(), ext.c_str());
                if (ext.compare("fbx") == 0)
                {
                    meshes.load_data(filepath.get_path_absolute());
                }
                //else if (!ext.compare("tga"))
                //{
                //    textures.load_data(filepath.get_path_absolute());
                //}
                //else if (!ext.compare("vs"))
                //{
                //}
            }
        }
    }

    Mesh* DataSystem::find_mesh(const FilePath& _path)
    {
        Mesh* mesh = nullptr;
        for (u32 i = 0; i < meshes.datas.size(); ++i)
        {
            DataHandle<Mesh>* mesh_handle = meshes.datas[i];
            if (!meshes.datas[i]->path.get_path_absolute().compare(_path.get_path_absolute()))
            {
                mesh =  mesh_handle->data;
            }
        }
        return mesh;
    }

    Mesh* DataSystem::find_mesh(const unique_id _id)
    {
        Mesh* mesh = nullptr;
        for (u32 i = 0; i < meshes.datas.size(); ++i)
        {
            DataHandle<Mesh>* mesh_handle = meshes.datas[i];
            if (meshes.datas[i]->id == _id)
            {
                mesh = mesh_handle->data;
            }
        }
        return mesh;
    }
}