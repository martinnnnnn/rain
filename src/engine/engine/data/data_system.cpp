#include "data_system.h"

#include <filesystem>

#include "core/core.h"

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
                    handle<Mesh> new_mesh_handle = load_data<Mesh>(filepath, core::uuid::random());
                    add_data<Mesh>(new_mesh_handle);
                }

            }
        }
    }
}