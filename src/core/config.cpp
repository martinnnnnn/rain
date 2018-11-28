#include "config.h"

#include "win32/win32_application.h"
#include "serializer/archivist.h"
#include "serializer/archivist_json.h"

namespace rain
{
    void Config::init(const std::string& _path)
    {
        file.open(_path);
        JsonReader::read_config(file.read(), *this);
        //std::fstream file = std::fstream(_path.c_str());
        //json_pupper* p = new json_pupper(file, pupper::IO::READ);
        //pup(p, *this, var_info("config"));
    }

    void archive(archivist* p_, Config& _config, const var_info& info)
    {
        archive(p_, _config.engine_name, var_info("engine_name")); 
        archive(p_, _config.data_root, var_info("data_root"));
        archive(p_, _config.starting_world, var_info("starting_world"));
        archive(p_, _config.screen_width, var_info("screen_width"));
        archive(p_, _config.screen_height, var_info("screen_height"));
        archive(p_, _config.full_screen, var_info("full_screen"));
    }

}