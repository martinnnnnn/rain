#include "config.h"

namespace rain::engine
{
	void Config::init(const std::string& _path)
	{
		file.open(_path);
		JsonReader::read_config(file.read(), *this);
	}
}