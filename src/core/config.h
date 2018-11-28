 #pragma once

#include <vector>
#include <string>
#include <map>
#include <cassert>

#include "core/singleton.h"
#include "core/file.h"
#include "core/string.h"
#include "win32/win32_application.h"
#include "core/json_reader.h"
#include "serializer/var_info.h"

namespace rain
{
    struct pupper;

    struct Config
    {
        void init(const std::string& _path);

        File file;
        std::string engine_name;
        std::string data_root;
        std::string starting_world;
        u32 screen_width;
        u32 screen_height;
        bool full_screen;
    };

    void pup(pupper* p_, Config& _config, const var_info& info);
}

#define RAIN_CONFIG rain::Application::get().config
