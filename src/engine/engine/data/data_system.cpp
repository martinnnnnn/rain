#include "data_system.h"

#include <filesystem>

#include "core/core.h"

#include "engine/gfx/ogl/ogl_texture.h"
#include "engine/core/profiler.h"
#include "engine/core/json_reader.h"

namespace rain::engine
{
    void data_system::reload_shaders()
    {
        u32 t_id = data_system_type_id::template get<Shader>();
        static_cast<handle_pool<Shader>*>(containers[t_id])->clear();

        for (const std::filesystem::directory_entry& p : std::filesystem::recursive_directory_iterator(root))
        {
            if (!p.is_directory())
            {
                std::string path = p.path().string();
                core::string::replace(path, "\\", "/");
                file_path filepath(path);

                std::string ext = filepath.get_extention();

                if (ext.compare("glsl.rain") == 0)
                {
                    RAIN_LOG("Reloading  shader %s", path.c_str());
					std::string content;
                    std::string vertex;
                    std::string fragment;
                    std::string geometry;
					core::File::read(path, content);
					json_reader::read_shader(content, filepath.get_directory(), vertex, fragment, geometry);
                    handle<Shader> new_shader_handle = load_data<Shader>(filepath, core::uuid::random(), vertex, fragment, geometry);
                    add_data<Shader>(new_shader_handle);
                }
            }
        }
    }

    void data_system::load_all_recursive(const std::string& r)
    {
        root = r;
        RAIN_PROFILE("Data loading");

        for (const std::filesystem::directory_entry& p : std::filesystem::recursive_directory_iterator(root))
        {
            if (!p.is_directory())
            {
                std::string path = p.path().string();
                core::string::replace(path, "\\", "/");
                file_path filepath(path);

                std::string ext = filepath.get_extention();
                if (ext.compare("fbx") == 0)
                {
                    RAIN_LOG("Loading model %s", path.c_str());
                    handle<Mesh> new_mesh_handle = load_data<Mesh>(filepath, core::uuid::random(), filepath);
                    add_data<Mesh>(new_mesh_handle);
                }
                if (ext.compare("glsl.rain") == 0)
                {
                    RAIN_LOG("Loading  shader %s", path.c_str());
					std::string content;
                    std::string vertex;
                    std::string fragment;
                    std::string geometry;
					core::File::read(path, content);
					json_reader::read_shader(content, filepath.get_directory(), vertex, fragment, geometry);
                    handle<Shader> new_shader_handle = load_data<Shader>(filepath, core::uuid::random(), vertex, fragment, geometry);
                    add_data<Shader>(new_shader_handle);
                }
				if (ext.compare("jpg") == 0 || ext.compare("png") == 0)
				{
					RAIN_LOG("Loading texture %s", path.c_str());
					handle<Texture> new_texture_handle = load_data<Texture>(filepath, core::uuid::random(), filepath.get_path_absolute().c_str(), GL_TEXTURE_2D);
					add_data<Texture>(new_texture_handle);
				}
            }
        }


        //std::vector<core::thread_pool::task_future<void>> v;
        //int i = 0;
        //std::mutex mut;
        //std::mutex meshmut;
        //for (const std::filesystem::directory_entry& p : std::filesystem::recursive_directory_iterator(root))
        //{
        //    if (!p.is_directory())
        //    {
        //        v.push_back(core::default_thread_pool::submit_job([&mut, &meshmut](std::string path, data_system* data_sys)
        //        {
        //            core::string::replace(path, "\\", "/");
        //            file_path filepath(path);

        //            std::string ext = filepath.get_extention();
        //            if (ext.compare("fbx") == 0)
        //            {
        //                RAIN_LOG("Loading model %s", path.c_str());
        //                meshmut.lock();
        //                handle<Mesh> new_mesh_handle = load_data<Mesh>(filepath, core::uuid::random(), filepath);
        //                meshmut.unlock();
        //                mut.lock();
        //                data_sys->add_data<Mesh>(new_mesh_handle);
        //                mut.unlock();
        //            }
        //            if (ext.compare("vert") == 0)
        //            {
        //                RAIN_LOG("Loading  shader %s", path.c_str());
        //                std::string vertex = filepath.get_directory() + filepath.get_name() + ".vert";
        //                std::string fragment = filepath.get_directory() + filepath.get_name() + ".frag";
        //                std::string geometry = filepath.get_directory() + filepath.get_name() + ".geom";
        //                handle<Shader> new_shader_handle = load_data<Shader>(filepath, core::uuid::random(), vertex, fragment, geometry);
        //                mut.lock();
        //                data_sys->add_data<Shader>(new_shader_handle);
        //                mut.unlock();
        //            }
        //            if (ext.compare("jpg") == 0 || ext.compare("png") == 0)
        //            {
        //                RAIN_LOG("Loading texture %s", path.c_str());
        //                handle<Texture> new_texture_handle = load_data<Texture>(filepath, core::uuid::random(), filepath.get_path_absolute().c_str(), GL_TEXTURE_2D);
        //                mut.lock();
        //                data_sys->add_data<Texture>(new_texture_handle);
        //                mut.unlock();
        //            }

        //        }, p.path().string(), this));
        //    }
        //}
        //for (auto& item : v)
        //{
        //    item.get();
        //}

    }
}