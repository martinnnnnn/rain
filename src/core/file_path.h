#pragma once

#include <string>

#include "serializer/var_info.h"

namespace rain
{
    struct archivist;

    class FilePath
    {
    public:
        FilePath() :
            path("")
        {

        }
        FilePath(const std::string& _path);
        std::string get_path_absolute();
        std::string get_path_relative();
        std::string get_name();
        std::string get_directory();

        static std::string get_path_relative(const std::string& _path);
        static std::string get_directory(const std::string& _path);
        static std::string get_name(const std::string& _path);
        static std::string get_exe_path();

    private:
        std::string path;
    };

    void archive(archivist* p_, FilePath& _file, const var_info& info);
}