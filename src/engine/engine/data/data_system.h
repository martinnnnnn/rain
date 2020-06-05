#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>

#include "core/core.h"
#include "data_handle.h"
#include "engine/core/context.h"
#include "geometry/mesh.h"
#include "texture/texture.h"
#include "engine/gfx/ogl/ogl_shader.h"
#include "engine/core/config.h"

namespace rain::engine
{
    using core::file_path;

    struct handle_pool_base
    {
        virtual ~handle_pool_base() {}
    };

    template<typename T>
    struct handle_pool : public handle_pool_base
    {
        void add_data(handle<T> new_data_handle)
        {
            handles.push_back(new_data_handle);
        }

        void clear()
        {
            for_each(handles.begin(), handles.end(), [](handle<T> handle)
            {
                delete handle.data;
            });
            handles.clear();
        }

        handle<T> const * find_data(const core::uuid& id)
        {
            for (u32 i = 0; i < handles.size(); ++i)
            {
                if (handles[i].id == id)
                {
                    return &(handles[i]);
                }
            }
            return nullptr;
        }

        handle<T> const * find_data(const file_path& path)
        {
            for (u32 i = 0; i < handles.size(); ++i)
            {
                if (handles[i].path.get_path_absolute().compare(path.get_path_absolute()) == 0)
                {
                    return &(handles[i]);
                }
            }
            return nullptr;
        }

        std::vector<handle<T>> handles;
    };

    class data_system
    {
        using data_system_type_id = core::type_id<struct DATA_SYSTEM_TYPE_ID>;

    public:
        template<typename T, typename... Args>
        static handle<T> load_data(const file_path& path, const core::uuid& id, Args &&... args)
        {
            handle<T> new_handle{ path, id };
            new_handle.data = new T();
            new_handle.data->load(std::forward<Args>(args)...);
            return new_handle;
        }

        template<typename T>
        void add_data_tf(handle<T> new_data_handle)
        {
            mut.lock();
            u32 t_id = data_system_type_id::template get<T>();
            if (t_id >= containers.size())
            {
                containers.resize(t_id + 1);
                containers[t_id] = new handle_pool<T>();
            }

            static_cast<handle_pool<T>*>(containers[t_id])->add_data(new_data_handle);
            mut.unlock();
        }

        template<typename T>
        void add_data(handle<T> new_data_handle)
        {
            u32 t_id = data_system_type_id::template get<T>();
            if (t_id >= containers.size())
            {
                containers.resize(t_id + 1);
                containers[t_id] = new handle_pool<T>();
            }

            static_cast<handle_pool<T>*>(containers[t_id])->add_data(new_data_handle);
        }

        template<typename T>
        handle<T> const * find_data(const core::uuid& id)
        {
            u32 t_id = data_system_type_id::template get<T>();
            return static_cast<handle_pool<T>*>(containers[t_id])->find_data(id);
        }

        template<typename T>
        handle<T> const * find_data(const file_path& path)
        {
            u32 t_id = data_system_type_id::template get<T>();
            return static_cast<handle_pool<T>*>(containers[t_id])->find_data(path);
        }

        // temp function until a tool parses the data directory, records [files path, uuid] and assigns these uuids to world entities that need them
        void load_all_recursive(const std::string& root);
        void reload_shaders();

        std::vector<handle_pool_base*> containers;
        std::mutex mut;
        std::string root;
    };


    //template<typename T>
    //struct DataInfo
    //{
    //    core::file_path path;
    //    core::uuid id;
    //    T* data;
    //};

    //struct DataSystem
    //{
    //    void update();

    //    void require_texture();
    //    void require_shader();
    //    void require_mesh();

    //    void load_texture(const file_path& path)
    //    {
    //        RAIN_LOG("Loading texture %s", path.get_path_absolute().c_str());


    //        Texture*

    //        handle<Texture> new_texture_handle = load_data<Texture>(filepath, core::uuid::random(), filepath.get_path_absolute().c_str(), GL_TEXTURE_2D);

    //        add_data<Texture>(new_texture_handle);
    //    }
    //    void load_shader();
    //    void reload_shader();
    //    void load_mesh();

    //private:
    //    std::vector<DataInfo<Texture>> texture_data;
    //    std::vector<DataInfo<Shader>> shader_data;
    //    std::vector<DataInfo<Mesh>> mesh_data;
    //    std::string root;
    //};

    template<typename T>
    struct DataInfo
    {
        using DataHandle = u32;

        DataHandle id;
        core::file_path path;
        T* data;
    };

    struct TexturesDatabase
    {
        static DataInfo<Texture>::DataHandle get_next_handle()
        {
            static DataInfo<Texture>::DataHandle handle = 0;
            return handle++;
        }

        void load(const core::file_path& path)
        {

        }

    private:
        std::vector<DataInfo<Texture>> texture_data;
    };

}
