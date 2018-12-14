#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include "core/logger.h"

namespace rain
{
    struct FilePath
    {


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
        std::string get_path_absolute() const;
        std::string get_path_relative() const;
        std::string get_name() const;
        std::string get_extention() const;
        std::string get_directory() const;

        static std::string get_path_relative(const std::string& _path);
        static std::string get_directory(const std::string& _path);
        static std::string get_name(const std::string& _path);
        static std::string get_exe_path();


    private:
        std::string path;
    };


    struct PathNode
    {
        enum class Type
        {
            DIRECTORY,
            FILE
        };

        PathNode()
            : type(Type::DIRECTORY)
            , value("")
            , parent(nullptr)
        {}

        PathNode(Type _type, const std::string& _value, PathNode* _parent)
            : type(_type)
            , value(_value)
            , parent(_parent)
        {}

        Type type;
        std::string value;
        PathNode* parent;
        std::vector<PathNode*> children;
    };

    namespace fs = std::filesystem;

    struct FilePathSystem
    {
        void init(const std::string _root)
        {
            for (auto& p : fs::recursive_directory_iterator(_root))
            {
                std::string path = p.path().string();
                String::replace(path, "\\", "/");
                FilePath filepath(path);
                if (!p.is_directory())
                {
                    paths.push_back(filepath);
                }
            }
        }

        std::vector<FilePath> paths;
    };

}