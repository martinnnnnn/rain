 #pragma once

#include <vector>
#include <string>
#include <map>
#include <cassert>

#include "core/core.h"

#define RAIN_CONFIG_FILE_NAME "/config.rain"

namespace rain::engine
{
    struct Config
    {
        void init();

        std::string engine_name;
		std::string data_root;
		std::string runtime_data_root;
        std::string starting_world;
        u32 screen_width;
        u32 screen_height;
        bool full_screen;
    };

}

