#include "file.h"


using namespace rain;



bool File::open(const std::string& _path)
{
	m_path = _path;
	m_file = std::fstream(m_path);

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
	m_file.close();
}