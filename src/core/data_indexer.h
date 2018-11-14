 #pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <map>
#include <cassert>

#include "core/singleton.h"
#include "core/file.h"
#include "core/string.h"
#include "win32/win32_application.h"
#include "core/json_reader.h"

namespace rain
{
    struct Config
    {
        void init(const std::string& _path);

        File file;
        std::string engine_name;
        std::string data_root;
        u32 screen_width;
        u32 screen_height;
        bool full_screen;
    };
}

#define RAIN_CONFIG rain::Application::get().config
