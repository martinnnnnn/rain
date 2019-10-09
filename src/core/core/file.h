#pragma once

#include <fstream>

#include "core/core.h"

namespace rain::core
{
    namespace file
    {
        inline void write(const std::string& path, u8* content, u32 size)
        {
            FILE* file;
            fopen_s(&file, path.c_str(), "wb");
            fwrite(content, sizeof(u8), size, file);
            fclose(file);
        }

        inline u32 read(const std::string& path, u8* content, u32 size)
        {
            FILE* file;
            fopen_s(&file, path.c_str(), "rb");
            const u32 size_read = fread(content, sizeof(u8), size, file);
            fclose(file);

            return size_read;
        }
    }

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
        file_path& get_path();
        std::fstream& get_stream();
        file_path filepath;

	private:
		std::fstream m_stream;
	};
}