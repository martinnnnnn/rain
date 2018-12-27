#include "path.h"

#include <cassert>

#include "core/types.h"
#include "core/string.h"

namespace rain::core
{
    FilePath::FilePath(const std::string& _path, const bool _isDirectory /*= false*/)
        : path(_path)
		, is_directory(_isDirectory)
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
        return path.substr(_root.length());
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
        return _path.substr(_root.length());
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
}