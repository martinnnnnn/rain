#include "file.h"

#include <cassert>

#include "win32/win32_helper.h"
#include "serializer/archivist.h"
#include "core/config.h"

namespace rain
{

    bool File::open(const std::string& _path, std::ios_base::openmode _mode)
    {
        m_path = FilePath(_path);
        m_stream = std::fstream(m_path.get_path_absolute().c_str(), _mode);
        return is_open();
    }

    bool File::reopen()
    {
        m_stream = std::fstream(m_path.get_path_absolute().c_str());
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

    rain::FilePath& File::get_path()
    {
        return m_path;
    }

    void archive(Archivist* p_, File& _file, const var_info& _info)
    {
        archive(p_, _file.get_path(), var_info("file"));
    }
}


