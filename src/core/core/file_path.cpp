#include "file_path.h"

#include <cassert>

#include "core/types.h"

namespace rain::core
{
    FilePath::FilePath(const std::string& _path)
        : path(_path)
    {
        std::for_each(path.begin(), path.end(), [](char & c)
        {
            c = ::tolower(c);
        });
    }

    std::string FilePath::get_path_absolute() const
    {
        return path;
    }

    std::string FilePath::get_path_relative(const std::string& _root) const
    {
        static u32 lenght = _root.length();
        return path.substr(lenght);
    }

    std::string FilePath::get_name() const
    {
        size_t lastSlash = path.find_last_of("/");
        return path.substr(lastSlash + 1, (path.length() - 1) - lastSlash);
    }

    std::string FilePath::get_extention() const
    {
        size_t lastSlash = path.find_first_of(".");
        return path.substr(lastSlash + 1, (path.length() - 1) - lastSlash);
    }

    std::string FilePath::get_directory() const
    {
        return path.substr(0, path.find_last_of("/"));
    }

    std::string FilePath::get_path_relative(const std::string& _path, const std::string& _root)
    {
        u32 lenght = _root.length();
        return _path.substr(lenght);
    }

    std::string FilePath::get_name(const std::string& _path)
    {
        size_t lastSlash = _path.find_last_of("/");
        return _path.substr(lastSlash + 1, (_path.length() - 1) - lastSlash);
    }

    std::string FilePath::get_directory(const std::string& _path)
    {
        return _path.substr(0, _path.find_last_of("/"));
    }

    void FilePathSystem::init(const std::string _root)
    {
        for (auto& p : fs::recursive_directory_iterator(_root))
        {
            std::string path = p.path().string();
            string::replace(path, "\\", "/");
            FilePath filepath(path);
            if (!p.is_directory())
            {
                paths.push_back(filepath);
            }
        }
    }

}