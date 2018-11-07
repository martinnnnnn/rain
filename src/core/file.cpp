#include "file.h"

#include <cassert>

using namespace rain;



bool File::open(const std::string& _path)
{
    m_path = _path;
    m_file = std::fstream(m_path);

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