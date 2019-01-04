#include "file.h"

#include <cassert>
#include <sstream>


namespace rain::core
{
    bool File::open(const std::string& _path, std::ios_base::openmode _mode)
    {
        
        filepath = FilePath(_path);
        m_stream = std::fstream(filepath.get_path_absolute().c_str(), _mode);
        return is_open();
    }

    bool File::reopen()
    {
        m_stream = std::fstream(filepath.get_path_absolute().c_str());
        return is_open();
    }

    bool File::is_open()
    {
        return m_stream.is_open();
    }

    std::fstream& File::get_stream()
    {
        return m_stream;
    }

    std::string File::read()
    {
        std::stringstream file_stream;
        file_stream << m_stream.rdbuf();
        return file_stream.str();
    }

    void File::close()
    {
        if (is_open())
        {
	        m_stream.close();
        }
    }

    FilePath& File::get_path()
    {
        return filepath;
    }

}


