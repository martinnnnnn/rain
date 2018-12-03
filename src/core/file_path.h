#pragma once

#include <string>


namespace rain
{
    struct Archivist;

    class FilePath
    {
    public:
        FilePath() :
            path("")
        {

        }
        FilePath(const std::string& _path);
        const FilePath& operator = (const FilePath& src)
        {
            path = src.path;
            return *this;
        }
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

}