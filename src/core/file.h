#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <algorithm>

#include "core/types.h"
#include "core/string.h"

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
}