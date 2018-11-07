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


        static std::string GetExePath()
        {
            char buffer[MAX_PATH];
            GetModuleFileName(NULL, buffer, MAX_PATH);
            std::string path = std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
            String::replace(path, "\\", "/");
            return path;
        }

	private:
		std::fstream m_file;
		std::string m_path;
	};
}