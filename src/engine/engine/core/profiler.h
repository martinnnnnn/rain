#pragma once

#include <chrono>
#include <string>

#include "core/core.h"
#include "glm.hpp"

namespace rain::engine
{

    struct console_profiler
    {
        console_profiler(const std::string& _message);
        ~console_profiler();

        std::string message;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;
    };

    struct profiler
    {
        profiler();
        f64 get_time();

        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;
    };


    struct window_profiler
    {
        window_profiler(const std::string& _message, const f32 _x, const f32 _y, const f32 _scale, const glm::vec4& _color);
        ~window_profiler();

        std::string message;
        f32 x;
        f32 y;
        f32 scale;
        glm::vec4 color;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;
    };
}

#define RAIN_PROFILE(msg) rain::engine::console_profiler p(msg)
#define RAIN_WPROFILE(msg, x, y, scale, color) rain::engine::window_profiler p(msg, x, y, scale, color)
