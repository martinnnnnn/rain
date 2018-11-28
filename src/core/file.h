#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include "core/types.h"
#include "core/string.h"
#include "serializer/var_info.h"
#include "serializer/archivist.h"
#include "file_path.h"
#include "core/logger.h"
#include "win32/win32_application.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

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

	private:
		std::fstream m_stream;
        FilePath m_path;
	};

    void archive(Archivist* p_, File& _file, const var_info& info);


    


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
                    RAIN_LOG("%s", boost::uuids::to_string(hello));
                    //std::cout << boost::uuids::to_string(hello) << '\n';
                }
            }
        }

        // map [uuid -> path]

    };


    
}