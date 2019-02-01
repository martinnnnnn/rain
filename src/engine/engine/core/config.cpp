#include "config.h"

//#include "engine/win32/win32_application.h"
#include "engine/core/json_reader.h"

namespace rain::engine
{
    void Config::init(const std::string& _path)
    {
        file.open(_path);
        json_reader::read_config(file.read(), *this);
    }
}