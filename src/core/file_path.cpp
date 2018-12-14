#include "file_path.h"

#include <cassert>

#include "win32/win32_helper.h"
#include "core/types.h"
#include "core/string.h"
#include "core/config.h"

namespace rain
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

    std::string FilePath::get_path_relative() const
    {
        static u32 lenght = RAIN_CONFIG->data_root.length();
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

    std::string FilePath::get_path_relative(const std::string& _path)
    {
        u32 lenght = RAIN_CONFIG->data_root.length();
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

    std::string FilePath::get_exe_path()
    {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string path = std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
        String::replace(path, "\\", "/");
        return path;
    }
}