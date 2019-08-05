#include "profiler.h"

#include "core/core.h"
#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"

namespace rain::engine
{
    console_profiler::console_profiler(const std::string& _message) : message(_message)
    {
        start = std::chrono::system_clock::now();
    }

    console_profiler::~console_profiler()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        RAIN_LOG_PROFILE((message + " : %.2fms").c_str(), elapsed_seconds.count() * 1000);
    }

    profiler::profiler()
    {
        start = std::chrono::system_clock::now();
    }

    f64 profiler::get_time()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        return elapsed_seconds.count();
    }

    window_profiler::window_profiler(const std::string& _message, const f32 _x, const f32 _y, const f32 _scale, const glm::vec4& _color)
        : message(_message)
        , x(_x)
        , y(_y)
        , scale(_scale)
        , color(_color)
    {
        start = std::chrono::system_clock::now();
    }

    window_profiler::~window_profiler()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        char buffer[512];
        core::string::print_to_buffer(buffer, 512, (message + " %f ms").c_str(), elapsed_seconds.count() * 1000.0);
        RAIN_RENDERER->draw_text_2d(buffer, x, y, scale, color);
    }
}

