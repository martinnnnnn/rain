#pragma once

#include <chrono>
#include <string>
#include <glm/glm.hpp>

#include "core/types.h"


namespace rain
{

    struct ConsoleProfiler
    {
        ConsoleProfiler(const std::string& _message);

        ~ConsoleProfiler();

        std::string message;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;

    };


    struct WindowProfiler
    {
        WindowProfiler(const std::string& _message, const f32 _x, const f32 _y, const f32 _scale, const glm::vec3& _color);

        ~WindowProfiler();

        std::string message;
        f32 x;
        f32 y;
        f32 scale;
        glm::vec3 color;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point end;

    };
}