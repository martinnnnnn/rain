#include "file_path.h"

#include <cassert>

#include "core/types.h"

namespace rain::core
{
    file_path::file_path(const std::string& _path)
        : path(_path)
    {
        std::for_each(path.begin(), path.end(), [](char & c)
        {
            c = ::tolower(c);
        });
    }

    std::string file_path::get_path_absolute() const
    {
        return path;
    }

    std::string file_path::get_path_relative(const std::string& _root) const
    {
        u32 lenght = _root.length();
        return path.substr(lenght);
    }

    std::string file_path::get_name() const
    {
        size_t last_slash = path.find_last_of("/");
        size_t point = path.find_first_of(".");
        u32 size_with_extension = (path.length() - 1) - last_slash;
        u32 extension_size = (path.length() - 1) - point;

        return path.substr(last_slash, size_with_extension - extension_size);
    }

    std::string file_path::get_extention() const
    {
        size_t point = path.find_first_of(".");
        return path.substr(point + 1, (path.length() - 1) - point);
    }

    std::string file_path::get_directory() const
    {
        return path.substr(0, path.find_last_of("/"));
    }

    std::string file_path::get_path_relative(const std::string& _path, const std::string& _root)
    {
        u32 lenght = _root.length();
        return _path.substr(lenght);
    }

    std::string file_path::get_name(const std::string& _path)
    {
        size_t lastSlash = _path.find_last_of("/");
        return _path.substr(lastSlash + 1, (_path.length() - 1) - lastSlash);
    }

    std::string file_path::get_directory(const std::string& _path)
    {
        return _path.substr(0, _path.find_last_of("/"));
    }

    void FilePathSystem::init(const std::string _root)
    {
        for (auto& p : fs::recursive_directory_iterator(_root))
        {
            std::string path = p.path().string();
            string::replace(path, "\\", "/");
            file_path filepath(path);
            if (!p.is_directory())
            {
                paths.push_back(filepath);
            }
        }
    }

}