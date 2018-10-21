#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "core/types.h"

namespace rain
{

	class File
	{
		enum class Mode
		{
			READ		= 1 << 0,				// open for reading only
			WRITE		= 1 << 1,				// open for write only
		};

		bool open(const std::string& _path);
		std::string read();
		void close();

	private:
		std::fstream m_file;
		std::string m_path;
	};
}