#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "core/types.h"
#include "core/string.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace fs = std::filesystem;

namespace rain
{

	class File
	{
	public:
		enum class Mode
		{
			READ		= 1 << 0,				// open for reading only
			WRITE		= 1 << 1,				// open for write only
		};

		bool open(const std::string& _path);
        bool reopen();
		bool is_open();
		std::string read();
		void close();
        std::string get_path();
        std::string get_directory();
        std::string get_name();

        static std::string get_directory(const std::string& _path);
        static std::string get_name(const std::string& _path);


        static std::string get_exe_path();
	private:
		std::fstream m_file;
		std::string m_path;
	};

    class FilePath
    {
    public:
        FilePath(const std::string& _path)
        {
            path = _path;
        }

        std::string get_name()
        {
            size_t lastSlash = path.find_last_of("/");
            return path.substr(lastSlash + 1, (path.length() - 1) - lastSlash);
        }

        std::string get_directory()
        {
            return path.substr(0, path.find_last_of("/"));
        }

        std::string path;
    };


    struct DataId
    {
        std::string path;
        boost::uuids::uuid id;
        // maybe find a way to get info about data and put it here for the editor ?
    };

    class DataSystem
    {
    public:
        DataSystem(const std::string& _dataRoot)
        {
            for (auto& p : fs::recursive_directory_iterator(_dataRoot))
            {
                std::string path = p.path().string();
                String::replace(path, "\\", "/");
                if (!p.is_directory())
                {
                    std::cout << path << '\n';

                    boost::uuids::uuid rain_uuid; 
                    boost::uuids::name_generator name_gen(rain_uuid);
                    boost::uuids::uuid hello = name_gen("theboostcpplibraries.com");
                    std::cout << boost::uuids::to_string(hello) << '\n';
                }
            }
        }

        // map [uuid -> path]

    };


    
}