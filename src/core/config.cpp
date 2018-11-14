#include "config.h"

#include "win32/win32_application.h"


namespace rain
{
    void Config::init(const std::string& _path)
    {
        file.open(_path);
        JsonReader::read_config(file.read(), *this);
    }
}