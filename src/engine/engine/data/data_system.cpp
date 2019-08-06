#include "data_system.h"

#include <filesystem>

#include "core/core.h"

#include "engine/gfx/ogl/ogl_texture.h"

namespace rain::engine
{
    void data_system::load_all_recursive(const std::string& root)
    {
        for (auto& p : std::filesystem::recursive_directory_iterator(root))
        {
            std::string path = p.path().string();
            core::string::replace(path, "\\", "/");
            file_path filepath(path);
            if (!p.is_directory())
            {
                std::string ext = filepath.get_extention();
                if (ext.compare("fbx") == 0)
                {
                    RAIN_LOG("Loading model %s", path.c_str());
                    handle<Mesh> new_mesh_handle = load_data<Mesh>(filepath, core::uuid::random(), filepath);
                    add_data<Mesh>(new_mesh_handle);
                }
                if (ext.compare("vs") == 0)
                {
                    RAIN_LOG("Loading  shader %s", path.c_str());
                    std::string vertex = filepath.get_directory() + filepath.get_name() + ".vs";
                    std::string fragment = filepath.get_directory() + filepath.get_name() + ".fs";
                    std::string geometry = filepath.get_directory() + filepath.get_name() + ".ge";
                    handle<Shader> new_shader_handle = load_data<Shader>(filepath, core::uuid::random(), vertex, fragment, geometry);
                    add_data<Shader>(new_shader_handle);
                }
                if (ext.compare("jpg") == 0)
                {
                    RAIN_LOG("Loading texture %s", path.c_str());
                    handle<Texture> new_texture_handle = load_data<Texture>(filepath, core::uuid::random(), filepath.get_path_absolute().c_str(), GL_TEXTURE_2D);
                    add_data<Texture>(new_texture_handle);
                }
                
            }
        }
    }
}