#include "profiler.h"

#include "core/core.h"
#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"

namespace rain::engine
{
    ConsoleProfiler::ConsoleProfiler(const std::string& _message) : message(_message)
    {
        start = std::chrono::system_clock::now();
    }

    ConsoleProfiler::~ConsoleProfiler()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        RAIN_LOG_PROFILE((message + " %f").c_str(), elapsed_seconds.count() * 1000);
    }

    WindowProfiler::WindowProfiler(const std::string& _message, const f32 _x, const f32 _y, const f32 _scale, const glm::vec3& _color)
        : message(_message)
        , x(_x)
        , y(_y)
        , scale(_scale)
        , color(_color)
    {
        start = std::chrono::system_clock::now();
    }

    WindowProfiler::~WindowProfiler()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        //char m[1024];
        //String::print_to_buffer(header_buffer, sizeof(header_buffer), "%s ", _level);
        RAIN_RENDERER->draw_text_2d("hello", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        RAIN_LOG_PROFILE((message + " %f").c_str(), elapsed_seconds.count() * 1000);
    }
}

