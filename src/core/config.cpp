#include "config.h"

#include "win32/win32_application.h"
#include "serializer/pupper.h"
#include "serializer/pupper_json.h"

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

    void pup(pupper* p_, Config& _config, const var_info& info)
    {
        pup(p_, _config.engine_name, var_info("engine_name")); 
        pup(p_, _config.data_root, var_info("data_root"));
        pup(p_, _config.starting_world, var_info("starting_world"));
        pup(p_, _config.screen_width, var_info("screen_width"));
        pup(p_, _config.screen_height, var_info("screen_height"));
        pup(p_, _config.full_screen, var_info("full_screen"));
    }

}