#include "file.h"

#include <cassert>

#include "win32/win32_helper.h"

namespace rain
{

    bool File::open(const std::string& _path)
    {
        m_path = _path;
        m_file = std::fstream(m_path.c_str());
        return is_open();
    }

    bool File::reopen()
    {
        m_file = std::fstream(m_path);
        return is_open();
    }

    bool File::is_open()
    {
        return m_file.is_open();
    }

    std::string File::read()
    {
        std::stringstream file_stream;
        file_stream << m_file.rdbuf();
        return file_stream.str();
    }

    void File::close()
    {
        if (is_open())
        {
	        m_file.close();
        }
    }

    std::string File::get_path()
    {
        return m_path;
    }
    std::string File::get_name()
    {
        size_t lastSlash = m_path.find_last_of("/");
        return m_path.substr(lastSlash + 1, (m_path.length() - 1) - lastSlash);
    }

    std::string File::get_directory()
    {
        return m_path.substr(0, m_path.find_last_of("/"));
    }

    std::string File::get_name(const std::string& _path)
    {
        size_t lastSlash = _path.find_last_of("/");
        return _path.substr(lastSlash + 1, (_path.length() - 1) - lastSlash);
    }

    std::string File::get_directory(const std::string& _path)
    {
        return _path.substr(0, _path.find_last_of("/"));
    }

    std::string File::get_exe_path()
    {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string path = std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
        String::replace(path, "\\", "/");
        return path;
    }
}


