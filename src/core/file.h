#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "core/types.h"
#include "core/string.h"
#include "file_path.h"
#include "core/logger.h"
#include "win32/win32_application.h"

namespace fs = std::filesystem;

namespace rain
{
    struct Archivist;

	class File
	{
	public:

		enum class Mode
		{
			READ		= 1 << 0,				// open for reading only
			WRITE		= 1 << 1,				// open for write only
		};

		bool open(const std::string& _path, std::ios_base::openmode _mode = std::fstream::in);
        bool reopen();
		bool is_open();
		std::string read();
		void close();
        FilePath& get_path();
        std::fstream& get_stream();
        FilePath filepath;

	private:
		std::fstream m_stream;
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
                FilePath filepath(path);
                if (!p.is_directory())
                {
                    RAIN_LOG("%s", filepath.get_path_relative().c_str());
                }
            }
        }

        // map [uuid -> path]

    };


    
}