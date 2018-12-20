#pragma once

#include <fstream>

#include "core/types.h"
#include "core/string.h"
#include "file_path.h"


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
}