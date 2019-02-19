#pragma once

#include <vector>
#include <string>
#include <unordered_map>

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

    struct id_container
    {
        virtual ~id_container() {}
    };

    template<typename T>
    struct handle_pool : public id_container
    {
        void add_data(handle<T> new_data_handle)
        {
            handles.push_back(new_data_handle);
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
                if (handles[i].path.get_path_absolute().compare(path.get_path_absolute()))
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
        void add_data(handle<T> new_data_handle)
        {
            u32 t_id = core::type_id<struct DATA_SYSTEM> ::get<T>();
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
            u32 t_id = core::type_id<struct DATA_SYSTEM>::get<T>();
            return static_cast<handle_pool<T>*>(containers[t_id])->find_data(id);
        }

        template<typename T>
        handle<T> const * find_data(const file_path& path)
        {
            u32 t_id = core::type_id<struct DATA_SYSTEM>::get<T>();
            return static_cast<handle_pool<T>*>(containers[t_id])->find_data(path);
        }

        // temp function until a tool parses the data directory, records [files path, uuid] and assigns these uuids to world entities that need them
        void load_all_recursive(const std::string& root);
        
        std::vector<id_container*> containers;
    };
}
